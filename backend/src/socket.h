#ifndef DMCR_SOCKET_H
#define DMCR_SOCKET_H

#include <cstdint>
#include <string>
#include <mutex>
#include <stdexcept>

#include <netinet/in.h>
#include <google/protobuf/message.h>
#include "dmcr_protocol.pb.h"

#include "itaskprovider.h"
#include "itasklistener.h"
#include "renderer.h"

namespace dmcr {

/*! \brief Exception thrown when socket problems happen
  */
class SocketException : public std::runtime_error {
public:
    /*! \skip
      */
    SocketException(const std::string& m) :
        std::runtime_error(m)
    {}
};

enum PacketId { Packet_BackendHandshake = 1, Packet_ConnectionResult = 2,
                Packet_NewTask = 3, Packet_RenderedData = 4 };

/*! \brief Backend-controller communication socket

   An abstracted TCP-based communication method between the backend and the
   controller.
*/
class Socket : public ITaskProvider {
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
    void setTaskListener(ITaskListener *listener);
    void setSocketListener(ISocketListener *listener);

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
                           uint32_t iterations_done, dmcr::Color *data);
    
    /*! \brief Called on task completion by the task listener
     * 
     * Reads the render result and sends it to the controller over the socket.
     * \param result Render result to transmit
     */
    virtual void onTaskCompleted(uint32_t task_id, 
                                 dmcr::RenderResultPtr result);


private:
    std::string m_hostname;
    in_port_t m_port;
    ITaskListener *m_task_listener;
    ISocketListener *m_socket_listener;
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
