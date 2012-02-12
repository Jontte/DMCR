#ifndef DMCR_ITASKPROVIDER_H
#define DMCR_ITASKPROVIDER_H

#include "renderer.h"

namespace dmcr {

class ITaskProvider {
public:
    virtual void onTaskCompleted(uint32_t task_id,
                                 dmcr::RenderResultPtr result) = 0;
};

}

#endif
