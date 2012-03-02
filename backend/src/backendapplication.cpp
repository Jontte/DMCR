#include "backendapplication.h"
#include "dummyscene.h"
#include <iostream>

using namespace dmcr;

static SceneFactory scene_factory = [](const std::string& str) {
    auto scene = std::make_shared<DummyScene>();
    scene->loadFromString(str);
    return scene;
};

BackendApplication::BackendApplication()
: m_task_manager(scene_factory)
{
}

void BackendApplication::connect(const std::string& hostname, int port) {
    m_socket = dmcr::make_unique<Socket>(hostname, port);
    m_socket->setSocketListener(this);
    m_socket->setTaskListener(&m_task_manager);
    m_socket->connect();
}

void BackendApplication::start() {
    m_socket->run();
}

void BackendApplication::onConnectionResult(ITaskProvider* provider,
                                            ConnectionResult result)
{
    (void)provider;
    
    if (result == ConnectionResult_Success) {
        std::cout << "Connection succesful." << std::endl;
    } else if (result == ConnectionResult_InvalidKey) {
        throw SocketException("Frontend refused connection: invalid key");
    } else if (result == ConnectionResult_ConnectionFailed) {
        throw SocketException("Frontend refused connection: unknown reason");
    } else {
        throw std::runtime_error("Unknown ConnectionResult");
    }
}
