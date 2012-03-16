/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_ITASKPROVIDER_H
#define DMCR_ITASKPROVIDER_H

#include "renderer.h"

namespace dmcr {

/*! \brief Interface implemented by classes that provide new tasks
 */
class ITaskProvider {
public:
    /*! \brief Event called by task listener when a task is completed
     * 
     * The task provider will then pass on the result or use it somehow.
     * \param task_id ID number of completed task. This is originally given
     *                by the provider in the onNewTask event
     * \param result Result of the rendering
     */
    virtual void onTaskCompleted(uint32_t task_id,
                                 const dmcr::RenderResultPtr& result) = 0;
};

}

#endif
