/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_TASK_H
#define DMCR_TASK_H

#include "scene.h"
#include <memory>
#include "renderer.h"

namespace dmcr {
    
class TaskManager;

/*! \brief Represents a running task in the system */
class Task
{
public:
    /*! \brief Construct a new task
     * 
     * Tasks are only intended to be constructed by TaskManager.
     * 
     * \param width Width of result image
     * \param height Height of result image
     * \param iterations Iterations to be calculated
     * \param scene Scene object to be rendered
     * \param mgr Task manager that owns this task
     */
    Task(uint16_t width, uint16_t height, uint32_t iterations,
         dmcr::ScenePtr scene, TaskManager *mgr);
    
    /*! \brief Start the task
     * 
     * This will spawn a new thread and run the task. Doesn't block.
     */
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
    uint32_t iterations() const {
        return m_iterations;
    }

    /*! \brief Get the result of the rendering task
     * 
     * This method will return the result of the rendering once 
     * the processing is complete. The thread will inform the task manager
     * once it is.
     */
    dmcr::RenderResultPtr renderResult() const {
        return m_result;
    }
    
    /*! \skip */
    void onThreadCompleted(dmcr::RenderResultPtr);
    
private:
    dmcr::ScenePtr m_scene;
    dmcr::TaskManager *m_manager;
    dmcr::RenderResultPtr m_result;
    uint16_t m_width, m_height;
    uint32_t m_iterations;
};

}

#endif // DMCR_TASK_H
