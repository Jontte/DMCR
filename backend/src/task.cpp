/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "task.h"
#include "renderer.h"
#include "taskmanager.h"
#include <thread>

#include "threadparallelrenderer.h"

class TaskThread {
public:
    TaskThread(dmcr::Task *task, dmcr::TaskManager *mgr) 
    : m_task(task), m_mgr(mgr)
    {
        
    }
    
    void operator ()() const
    {
        dmcr::ThreadParallelRenderer renderer(m_task->scene());
        dmcr::RenderResultPtr result = renderer.render(m_task->width(),
                                                       m_task->height(),
                                                       0, m_task->width()-1,
                                                       0, m_task->height()-1);
        m_task->onThreadCompleted(result);
    }
    
private:
    dmcr::Task *m_task;
    dmcr::TaskManager *m_mgr;
};

dmcr::Task::Task(uint16_t width, uint16_t height, uint32_t iterations, 
           dmcr::ScenePtr scene, dmcr::TaskManager *mgr)
: m_scene(scene), m_manager(mgr), m_width(width), m_height(height)
{
    (void)iterations;
}

void dmcr::Task::run()
{
    std::thread thr(TaskThread(this, m_manager));
    thr.detach();
}

void dmcr::Task::onThreadCompleted(dmcr::RenderResultPtr result)
{
    m_result = result;
    m_manager->onTaskCompleted(this);
}
