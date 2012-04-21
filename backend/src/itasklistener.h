/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_ITASKLISTENER_H
#define DMCR_ITASKLISTENER_H

namespace dmcr {

class ITaskProvider;

/*! \brief Interface called by ITaskProvider when events happen */
class ITaskListener {
public:

    /*! Called when a task provider has a new rendering task for the listener.
      \param provider Task provider object
      \param task_id ID number of the new task. Must be retained and passed in
                     the onTaskCompleted event
      \param width Desired rendering width
      \param height Desired rendering height
      \param iterations Number of iterations the backend should render per pixel
      \param scene The scene to be rendered in DMCR scene description language
      */
    virtual void onNewTask(ITaskProvider *provider, uint32_t task_id,
                           uint16_t width, uint16_t height, uint32_t iterations, 
                           const std::string& scene) = 0;
    virtual void onTaskRemoved(ITaskProvider *provider, uint32_t task_id) = 0;

};

enum ConnectionResult { ConnectionResult_Success = 0,
                        ConnectionResult_InvalidKey = 1,
                        ConnectionResult_ConnectionFailed = 2 };

/*! \brief Interface called by Socket when events happen */
class ISocketListener {
public:
    /*! Called when the connection is established and the controller replies
      to the handshake.
      \param socket Receiving socket
      \param result Connection result
      */
    virtual void onConnectionResult(ITaskProvider *provider,
                                    ConnectionResult result) = 0;
};

}

#endif
