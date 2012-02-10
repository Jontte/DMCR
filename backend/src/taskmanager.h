#ifndef DMCR_TASKMANAGER_H
#define DMCR_TASKMANAGER_H

#include "socket.h"
#include <string>
#include <functional>
#include "scene.h"
#include <list>
#include "task.h"

namespace dmcr {
    
class TaskManagerException : public std::runtime_error {
public:
    /*! \skip
      */
    TaskManagerException(const std::string& m) :
        std::runtime_error(m)
    {}
};

typedef std::function<dmcr::ScenePtr(const std::string&)> SceneFactory;

class TaskManager : public dmcr::IBackendSocketListener
{
public:
    TaskManager(SceneFactory factory);
    
    virtual void onConnectionResult(Socket* socket, ConnectionResult result);
    virtual void onNewTask(Socket* socket, uint16_t width, uint16_t height, 
                           uint32_t iterations, const std::string& scene_str);
    
    virtual void onTaskCompleted(Task *task);
    
private:
    SceneFactory m_scene_factory;
    std::list<TaskPtr> m_tasks;
};

}

#endif // DMCR_TASKMANAGER_H
