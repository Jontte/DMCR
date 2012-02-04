#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "dmcr_protocol.pb.h"
#include <cstdio>
#include <memory>

dmcr::Socket::Socket(const std::string &hostname, in_port_t port)
    : m_hostname(hostname), m_port(port), m_listener(NULL),
      m_fd(0)
{
}

dmcr::Socket::~Socket()
{
    close(m_fd);
}

void dmcr::Socket::setListener(IBackendSocketListener *listener)
{
    m_listener = listener;
}

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

void dmcr::Socket::sendHandshakePacket()
{
    dmcr::Packet::BackendHandshake packet;
    packet.set_protocol_version(0);
    packet.set_description("DMCR/1.0");
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
    send(m_fd, &header_len, sizeof(uint32_t), MSG_MORE);
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

void dmcr::Socket::readPacket()
{
    // Protobuf messages are always variable length, so we need to send the
    // packet size separately.
    // This is a bit kludgy, as the protocol is currently as follows:
    // 1. header length as uint32_t 2. protobuf header with message length
    // 3. actual protobuf message
    // But it works.

    char len_buffer[4];
    uint32_t read_len = 0;
    while (read_len < 4)
        read_len += recv(m_fd, len_buffer+read_len, 4-read_len, MSG_WAITALL);

    uint32_t header_len = ntohl(*((uint32_t*)len_buffer));

    char buffer[header_len];
    read_len = 0;
    while (read_len < header_len)
        read_len += recv(m_fd, buffer+read_len, header_len-read_len,
                         MSG_WAITALL);

    dmcr::Packet::PacketHeader header;
    header.ParseFromArray(buffer, header_len);

    std::unique_ptr<char[]> buffer2(new char[header.length()]);

    read_len = 0;
    while (read_len < header.length())
        read_len += recv(m_fd, buffer2.get()+read_len,
                         header.length()-read_len, 0);

    PacketId pid = (PacketId)header.id();

#define PACKET_CASE(name) case Packet_##name: { dmcr::Packet::name msg;\
    msg.ParseFromArray(buffer2.get(), header.length()); handle##name(msg);\
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
    if (m_listener)
        m_listener->onConnectionResult(this, (ConnectionResult)msg.result());
}

void dmcr::Socket::run()
{
    for (;;)
        readPacket();
}

void dmcr::Socket::handleNewTask(const dmcr::Packet::NewTask &msg)
{
    if (m_listener)
        m_listener->onNewTask(this, msg.width(), msg.height(),
                              msg.iterations(), msg.scene());
}

void dmcr::Socket::sendRenderedImage(uint32_t task, uint32_t width,
                                     uint32_t height, uint32_t iterations_done,
                                     float *data)
{
    int data_len = width*height*3*sizeof(float);

    dmcr::Packet::RenderedData packet;
    packet.set_width(width);
    packet.set_height(height);
    packet.set_id(task);
    packet.set_iterations_done(iterations_done);
    packet.set_data_length(data_len);

    std::lock_guard<std::mutex> G(m_mutex);

    sendPacketUnsafe(Packet_RenderedData, packet);

    send(m_fd, data, data_len, 0);
}
