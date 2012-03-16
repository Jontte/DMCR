/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

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
    auto scene = m_scene_factory(scene_str);
    
    auto task = dmcr::make_unique<Task>(width, height, iterations, scene, this);
    dmcr::Task *task_ptr = task.get();
    
    {
        std::lock_guard<std::mutex> G(m_mutex);
        
        TaskData data;
        data.task = std::move(task);
        data.provider = provider;
        data.id = id;
        
        m_tasks.push_front(std::move(data));
    }
    
    // Hack, but should be safe as if an exception is raised this line
    // won't be run
    task_ptr->run();
}

#include <iostream>
void dmcr::TaskManager::onTaskCompleted(dmcr::Task *task)
{
    TaskData data;
    {
        std::lock_guard<std::mutex> G(m_mutex);
        bool found = false;
        
        for (const auto& i = m_tasks.begin(); i != m_tasks.end();) {
            if ((*i).task.get() == task) {
                data = std::move(*i);
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
