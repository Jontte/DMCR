#include "taskmanager.h"
#include "scene.h"
#include "loadbalancer.h"
#include "task.h"

dmcr::TaskManager::TaskManager(SceneFactory factory)
: IBackendSocketListener()
{
    m_scene_factory = factory;
}


void dmcr::TaskManager::onConnectionResult(dmcr::Socket* socket, 
                                           dmcr::ConnectionResult result)
{
    (void)socket;
    
    switch (result) {
        case dmcr::ConnectionResult_ConnectionFailed:
            throw TaskManagerException("Controller refused connection");
        case dmcr::ConnectionResult_InvalidKey:
            throw TaskManagerException("Invalid key");
        default: {}
    }
}

void dmcr::TaskManager::onNewTask(dmcr::Socket* socket, uint16_t width, 
                                  uint16_t height, uint32_t iterations, 
                                  const std::string& scene_str)
{
    (void)socket;

    dmcr::ScenePtr scene = m_scene_factory(scene_str);
    
    dmcr::TaskPtr task(new dmcr::Task(width, height, iterations, scene, this));
    m_tasks.push_front(task);
    task->run();
}

void dmcr::TaskManager::onTaskCompleted(dmcr::Task *task)
{
    for (auto i = m_tasks.begin(); i != m_tasks.end();) {
        if ((*i).get() == task)
            i = m_tasks.erase(i);
        else
            ++i;
    }
    
    /* TODO all kinds of data sending etc. */
}
