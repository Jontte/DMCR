#ifndef DMCR_TASK_H
#define DMCR_TASK_H

#include "scene.h"
#include <memory>

namespace dmcr {
    
class TaskManager;

class Task
{
public:
    Task(uint16_t width, uint16_t height, uint32_t iterations,
         dmcr::ScenePtr scene, TaskManager *mgr);
    
    void run();
    
private:
    dmcr::ScenePtr m_scene;
    dmcr::TaskManager *m_manager;
};

typedef std::shared_ptr<Task> TaskPtr;

}

#endif // DMCR_TASK_H
