#ifndef DMCR_SOCKET_H
#define DMCR_SOCKET_H

#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <google/protobuf/message.h>
#ifdef DMCR_THREADED
#include <pthread.h>
#endif

namespace dmcr {

class SocketException {
public:
    SocketException(const std::string& m) :
        message(m)
    {}

    const std::string message;
};

enum ConnectionResult { ConnectionResult_Success, ConnectionResult_InvalidKey,
                        ConnectionResult_ConnectionFailed };

enum PacketId { Packet_BackendHandshake = 1 };

class Socket;

class IBackendSocketListener {
public:
    virtual void onConnectionResult(Socket *socket,
                                    ConnectionResult result,
                                    const std::string& message) = 0;
    virtual void onNewTask(Socket *socket, uint16_t width, uint16_t height,
                           uint32_t iterations, const std::string& scene) = 0;

};

/* dmcr::Socket
   Abstracted TCP-based communication method between the backend and controller.
*/
class Socket {
public:
    Socket(const std::string& hostname, in_port_t port);
    ~Socket();

    void setListener(IBackendSocketListener *listener);
    void connect();

private:
    std::string m_hostname;
    in_port_t m_port;
    IBackendSocketListener *m_listener;
#ifdef DMCR_THREADED
    pthread_spinlock_t m_spinlock;
#endif

    int m_fd;
    uint32_t m_seq;

    void sendHandshakePacket();
    void sendPacket(PacketId, const ::google::protobuf::Message &);
};

}

#endif
