#ifndef DMCR_SOCKET_H
#define DMCR_SOCKET_H

#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <google/protobuf/message.h>
#include "dmcr_protocol.pb.h"
#include <mutex>

namespace dmcr {

/*! \brief Exception thrown when socket problems happen
  */
class SocketException {
public:
    /*! \skip
      */
    SocketException(const std::string& m) :
        message(m)
    {}

    /*! Message describing what happened */
    const std::string message;
};

enum ConnectionResult { ConnectionResult_Success, ConnectionResult_InvalidKey,
                        ConnectionResult_ConnectionFailed };

enum PacketId { Packet_BackendHandshake = 1, Packet_ConnectionResult = 2,
                Packet_NewTask = 3, Packet_RenderedData = 4 };

class Socket;

/*! \brief Interface called by Socket when events happen */
class IBackendSocketListener {
public:
    /*! Called when the connection is established and the controller replies
      to the handshake.
      \param socket Receiving socket
      \param result Connection result
      */
    virtual void onConnectionResult(Socket *socket,
                                    ConnectionResult result) = 0;
    /*! Called when the controller has a new rendering task for backend.
      \param socket Receiving socket
      \param width Desired rendering width
      \param height Desired rendering height
      \param iterations Number of iterations the backend should render per pixel
      \param scene The scene to be rendered in DMCR scene description language
      */
    virtual void onNewTask(Socket *socket, uint16_t width, uint16_t height,
                           uint32_t iterations, const std::string& scene) = 0;

};

/*! \brief Backend-controller communication socket

   An abstracted TCP-based communication method between the backend and the
   controller.
*/
class Socket {
public:
    /*! \brief Constructs a socket
      Constructs a new communication socket.
      \param hostname Hostname to connect to
      \param port TCP port to connect to
      */
    Socket(const std::string& hostname, in_port_t port);
    ~Socket();

    /*! \brief Set event listener

      Set the event listener the methods of which will be called when certain
      events happen. The event listener must be an instance of a class that
      inherits IBackendSocketListener.
      \param listener Event listener
      */
    void setListener(IBackendSocketListener *listener);

    /*! \brief Connect to host

      Attempts to connect to specified hostname and port. If the hostname
      resolves to multiple IP addresses, they will be tried in sequence until
      the connection succeeds or all have been tried.
      \throws SocketException
      */
    void connect();

    /*! \brief Enter socket loop

      Enters a blocking loop reading and processing incoming messages.
      */
    void run();

    /*! \brief Send rendered image data

      Sends a rendered image to the controller.
      \param task ID number of the relevant task
      \param width Width of image in pixels
      \param height Height of image in pixels
      \param iterations_done Number of iterations that have been calculated
      \param data Array of floats of size width*height*3
      */
    void sendRenderedImage(uint32_t task, uint32_t width, uint32_t height,
                           uint32_t iterations_done, float *data);

private:
    std::string m_hostname;
    in_port_t m_port;
    IBackendSocketListener *m_listener;
    std::mutex m_mutex;

    int m_fd;

    void sendHandshakePacket();
    void sendPacketUnsafe(PacketId, const ::google::protobuf::Message &);
    void sendPacket(PacketId, const ::google::protobuf::Message &);
    void readPacket();

    void handleConnectionResult(const dmcr::Packet::ConnectionResult&);
    void handleNewTask(const dmcr::Packet::NewTask&);
};

}

#endif
