#ifndef DMCR_TASKMANAGER_H
#define DMCR_TASKMANAGER_H

#include <string>
#include <functional>
#include "scene.h"
#include <list>
#include "task.h"
#include <mutex>
#include "renderer.h"
#include <list>
#include "itasklistener.h"

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

class TaskManager : public dmcr::ITaskListener
{
public:
    TaskManager(SceneFactory factory);
    
    virtual void onNewTask(ITaskProvider *provider, uint32_t id,
                           uint16_t width, uint16_t height, 
                           uint32_t iterations, const std::string& scene_str);
    
    virtual void onTaskCompleted(Task *task);
    
private:
    struct TaskData {
        TaskPtr task;
        ITaskProvider *provider;
        uint32_t id;
    };
    
    SceneFactory m_scene_factory;
    std::list<TaskData> m_tasks;
    std::mutex m_mutex;
};

}

#endif // DMCR_TASKMANAGER_H
