/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_TASKMANAGER_H
#define DMCR_TASKMANAGER_H

#include <functional>
#include <list>
#include <list>
#include <mutex>
#include <stdexcept>
#include <string>

#include "itasklistener.h"
#include "renderer.h"
#include "scene.h"
#include "task.h"
#include "unique_ptr"

namespace dmcr {
    
class ITaskProvider;
class Task;

class TaskManagerException : public std::runtime_error {
public:
    /*! \skip
      */
    TaskManagerException(const std::string& m) :
        std::runtime_error(m)
    {}
};

/*! Lambda that will be called with the scene string when a scene object
 * needs to be constructed. */
typedef std::function<dmcr::ScenePtr(const std::string&)> SceneFactory;

/*! \brief Manages tasks. Can receive tasks from multiple task providers. */
class TaskManager : public dmcr::ITaskListener
{
public:
    /*! \brief Construct a new task manager.
     * 
     * You should only ever need one task manager running simultaneously.
     * \param factory Scene factory lambda
     */
    TaskManager(SceneFactory factory);
    
    /*! \brief Called by a task provider when a new task is waiting
     * 
     * \param provider Pointer to task provider
     * \param id ID number of new task
     * \param width Width of render result
     * \param height Height of render result
     * \param iterations Number of iterations to calculate
     * \param scene_str Scene description string
     */
    virtual void onNewTask(ITaskProvider *provider, uint32_t id,
                           uint16_t width, uint16_t height, 
                           uint32_t iterations, const std::string& scene_str);
    
    /*! \skip */
    virtual void onTaskCompleted(Task *task);
    
private:
    struct TaskData {
        std::unique_ptr<Task> task;
        ITaskProvider *provider;
        uint32_t id;
    };
    
    SceneFactory m_scene_factory;
    std::list<TaskData> m_tasks;
    std::mutex m_mutex;
};

}

#endif // DMCR_TASKMANAGER_H
