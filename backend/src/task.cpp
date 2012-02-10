#include "task.h"

dmcr::Task::Task(uint16_t width, uint16_t height, uint32_t iterations, 
           dmcr::ScenePtr scene, dmcr::TaskManager *mgr)
: m_scene(scene), m_manager(mgr)
{
    (void)width;
    (void)height;
    (void)iterations;
}

void dmcr::Task::run()
{
    // TODO
}
