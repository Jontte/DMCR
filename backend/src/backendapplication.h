/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_BACKENDAPPLICATION_H
#define DMCR_BACKENDAPPLICATION_H

#include "taskmanager.h"
#include "unique_ptr"
#include "itasklistener.h"
#include "socket.h"

namespace dmcr
{

class BackendApplication : public ISocketListener
{
public:
    BackendApplication();

    void connect(const std::string& hostname, int port);

    void start();

    virtual void onConnectionResult(ITaskProvider *provider,
                                    ConnectionResult result);

private:
    TaskManager m_task_manager;
    std::unique_ptr<Socket> m_socket;
};

}

#endif // DMCR_BACKENDAPPLICATION_H
