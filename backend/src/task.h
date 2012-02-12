#ifndef DMCR_TASK_H
#define DMCR_TASK_H

#include "scene.h"
#include <memory>
#include "renderer.h"

namespace dmcr {
    
class TaskManager;

class Task
{
public:
    Task(uint16_t width, uint16_t height, uint32_t iterations,
         dmcr::ScenePtr scene, TaskManager *mgr);
    
    void run();
    
    dmcr::ScenePtr scene() const {
        return m_scene;
    }
    uint16_t width() const {
        return m_width;
    }
    uint16_t height() const {
        return m_height;
    }
    dmcr::RenderResultPtr renderResult() const {
        return m_result;
    }
    
    void onThreadCompleted(dmcr::RenderResultPtr);
    
private:
    dmcr::ScenePtr m_scene;
    dmcr::TaskManager *m_manager;
    dmcr::RenderResultPtr m_result;
    uint16_t m_width, m_height;
};

typedef std::shared_ptr<Task> TaskPtr;

}

#endif // DMCR_TASK_H
