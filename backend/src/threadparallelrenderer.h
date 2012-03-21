/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_THREADPARALLELRENDERER_H
#define DMCR_THREADPARALLELRENDERER_H

#include "scene.h"
#include "renderer.h"

namespace dmcr
{

class ThreadParallelRenderer : public Renderer
{
public:
    ThreadParallelRenderer(ScenePtr scene, uint32_t iterations);

    virtual
    std::shared_ptr<RenderResult> render(uint16_t h_res, uint16_t v_res,
                                         uint16_t left = 0, uint16_t right = 0,
                                         uint16_t top = 0, uint16_t bottom = 0)
    const;
    
private:
    uint32_t m_iterations;
};

}

#endif // DMCR_THREADPARALLELRENDERER_H
