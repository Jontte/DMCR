#ifndef DMCR_THREADPARALLELRENDERER_H
#define DMCR_THREADPARALLELRENDERER_H

#include "scene.h"
#include "renderer.h"
#include "loadbalancer.h"

namespace dmcr
{

class ThreadParallelRenderer : public Renderer
{
public:
    ThreadParallelRenderer(ScenePtr scene);

    virtual
    std::shared_ptr<RenderResult> render(uint16_t h_res, uint16_t v_res,
                                         uint16_t left = 0, uint16_t right = 0,
                                         uint16_t top = 0, uint16_t bottom = 0)
    const;
    
private:
    mutable dmcr::LoadBalancer m_balancer;
};

}

#endif // DMCR_THREADPARALLELRENDERER_H
