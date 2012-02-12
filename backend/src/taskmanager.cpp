#include "taskmanager.h"
#include "scene.h"
#include "loadbalancer.h"
#include "task.h"
#include "itaskprovider.h"

dmcr::TaskManager::TaskManager(SceneFactory factory)
: dmcr::ITaskListener()
{
    m_scene_factory = factory;
}

void dmcr::TaskManager::onNewTask(dmcr::ITaskProvider* provider, uint32_t id,
                                  uint16_t width,  uint16_t height, 
                                  uint32_t iterations, 
                                  const std::string& scene_str)
{
    dmcr::ScenePtr scene = m_scene_factory(scene_str);
    
    dmcr::TaskPtr task(new dmcr::Task(width, height, iterations, scene, this));
    
    {
        std::lock_guard<std::mutex> G(m_mutex);
        
        TaskData data;
        data.task = task;
        data.provider = provider;
        data.id = id;
        
        m_tasks.push_front(data);
    }
    
    task->run();
}

#include <iostream>
void dmcr::TaskManager::onTaskCompleted(dmcr::Task *task)
{
    TaskData data;
    {
        std::lock_guard<std::mutex> G(m_mutex);
        bool found = false;
        
        for (auto i = m_tasks.begin(); i != m_tasks.end();) {
            if ((*i).task.get() == task) {
                data = *i;
                m_tasks.erase(i);
                found = true;
                break;
            }
        }
        if (!found)
            throw std::runtime_error("onTaskCompleted on unexisting task!");
    }
    
    data.provider->onTaskCompleted(data.id, task->renderResult());
}
