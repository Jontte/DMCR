#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "dmcr_protocol.pb.h"
#include <cstdio>
#include <unistd.h>
#include <cerrno>
#include <vector>
#include <sys/utsname.h>

static int checkError(int r) {
    if (r == -1)
        throw dmcr::SocketException(strerror(errno));
    return r;
}

dmcr::Socket::Socket(const std::string &hostname, in_port_t port)
    : m_hostname(hostname), m_port(port), m_task_listener(NULL),
      m_socket_listener(NULL), m_fd(0)
{
}

dmcr::Socket::~Socket()
{
    close(m_fd);
}

void dmcr::Socket::setTaskListener(dmcr::ITaskListener *listener)
{
    m_task_listener = listener;
}

void dmcr::Socket::setSocketListener(dmcr::ISocketListener* listener)
{
    m_socket_listener = listener;
}

extern int snprintf(char *dst, const char *fmt, ...);

void dmcr::Socket::connect()
{
    // Resolve host
    struct addrinfo *addr;
    struct addrinfo hints;
    hints.ai_flags = AI_ADDRCONFIG;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL; hints.ai_addr = NULL; hints.ai_next = NULL;

    char port_str[10];
    snprintf(port_str, 10, "%d", m_port);

    int r = getaddrinfo(m_hostname.c_str(), port_str, &hints, &addr);
    if (r != 0)
        throw SocketException("Could not resolve hostname");

    int fd = 0;
    for (; addr != NULL; addr = addr->ai_next) {
        fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (!fd)
            continue;

        if (::connect(fd, addr->ai_addr, addr->ai_addrlen) == -1) {
            close(fd);
            continue;
        }

        break;
    }

    if (addr == NULL) {
        freeaddrinfo(addr);
        throw SocketException("Could not connect to host");
    }

    freeaddrinfo(addr);

    m_fd = fd;

    sendHandshakePacket();
}

static std::string getMachineDescription() {
    struct utsname data;
    if (uname(&data) == -1)
        return "DMCR/1.0 Unknown";

    std::string s = "DMCR/1.0 ";

    s.append(data.sysname);
    s.append("/");
    s.append(data.release);
    s.append(" ");
    s.append(data.nodename);
    s.append(" ");
    s.append(data.machine);

    return s;
}

void dmcr::Socket::sendHandshakePacket()
{
    dmcr::Packet::BackendHandshake packet;
    packet.set_protocol_version(0);
    packet.set_description(getMachineDescription());
    sendPacket(Packet_BackendHandshake, packet);
}

void dmcr::Socket::sendPacketUnsafe(PacketId id,
                              const ::google::protobuf::Message &msg)
{
    dmcr::Packet::PacketHeader header;
    header.set_length(msg.ByteSize());
    header.set_id((uint8_t)id);

    // First send the header length
    uint32_t header_len = htonl(header.ByteSize());
    checkError(send(m_fd, &header_len, sizeof(uint32_t), 0));
    // Then the header
    header.SerializeToFileDescriptor(m_fd);
    // And the message
    msg.SerializeToFileDescriptor(m_fd);
}

void dmcr::Socket::sendPacket(PacketId id,
                              const::google::protobuf::Message &msg)
{
    std::lock_guard<std::mutex> G(m_mutex);
    sendPacketUnsafe(id, msg);
}

static const uint32_t MAX_HEADER_LENGTH = 64;
void dmcr::Socket::readPacket()
{
    // Protobuf messages are always variable length, so we need to send the
    // packet size separately.
    // This is a bit kludgy, as the protocol is currently as follows:
    // 1. header length as uint32_t 2. protobuf header with message length
    // 3. actual protobuf message
    // But it works.
    
    /* Part 1 - Receive length of header as an uint32_t */
    char header_len_buf[4];
    uint32_t read_len = 0;
    while (read_len < 4)
        read_len += checkError(recv(m_fd, header_len_buf+read_len, 4-read_len, 
                                    MSG_WAITALL));

    uint32_t header_len = ntohl(*((uint32_t*)header_len_buf));
    if (header_len > MAX_HEADER_LENGTH)
        throw SocketException("Too long packet header");
    if (header_len == 0)
        throw SocketException("Zero length header");

    /* Part 2 - Receive header */
    std::vector<char> header_buf(header_len);
    read_len = 0;
    while (read_len < header_len)
        read_len += checkError(recv(m_fd, header_buf.data()+read_len, 
                                    header_len-read_len, MSG_WAITALL));

    dmcr::Packet::PacketHeader header;
    header.ParseFromArray(header_buf.data(), header_len);
    
    /* Part 3 - Receive packet */
    std::vector<char> packet_buf(header.length());
    read_len = 0;
    while (read_len < header.length())
        read_len += checkError(recv(m_fd, packet_buf.data()+read_len,
                                    header.length()-read_len, 0));
        
    /* Dispatch packet to handler */

    PacketId pid = (PacketId)header.id();

#define PACKET_CASE(name) case Packet_##name: { dmcr::Packet::name msg;\
    msg.ParseFromArray(packet_buf.data(), header.length()); handle##name(msg);\
    break; };

    switch (pid) {
    PACKET_CASE(ConnectionResult);
    PACKET_CASE(NewTask);
    case Packet_BackendHandshake:
    default:
        throw SocketException("Received unexpected packet");
    }
}

#include <iostream>
void dmcr::Socket::handleConnectionResult(const dmcr::Packet::ConnectionResult
                                          &msg)
{
    if (m_socket_listener)
        m_socket_listener->onConnectionResult(
            this, (ConnectionResult)msg.result());
}

void dmcr::Socket::run()
{
    for (;;)
        readPacket();
}

void dmcr::Socket::handleNewTask(const dmcr::Packet::NewTask &msg)
{
    if (m_task_listener)
        m_task_listener->onNewTask(this, msg.id(), msg.width(), msg.height(),
                                   msg.iterations(), msg.scene());
}

void dmcr::Socket::sendRenderedImage(uint32_t task, uint32_t width,
                                     uint32_t height, uint32_t iterations_done,
                                     std::vector<dmcr::Color> data)
{
    dmcr::Packet::RenderedData packet;
    packet.set_width(width);
    packet.set_height(height);
    packet.set_id(task);
    packet.set_iterations_done(iterations_done);
    packet.set_data_length(width*height*3*sizeof(uint32_t));

    std::vector<uint32_t> data_buf(width*height*3);
    for (uint32_t i = 0; i < width*height; ++i) {
        data_buf[3*i+0] = htonl((uint32_t)(data[i].r * 0xffffffff));
        data_buf[3*i+1] = htonl((uint32_t)(data[i].g * 0xffffffff));
        data_buf[3*i+2] = htonl((uint32_t)(data[i].b * 0xffffffff));
    }

    std::lock_guard<std::mutex> G(m_mutex);

    sendPacketUnsafe(Packet_RenderedData, packet);

    checkError(send(m_fd, data_buf.data(), width*height*3*sizeof(uint32_t), 0));
}

void dmcr::Socket::onTaskCompleted(uint32_t task_id,
                                   const dmcr::RenderResultPtr& result)
{
    sendRenderedImage(task_id, result->width(), result->height(), 1,
                      result->data());
}
