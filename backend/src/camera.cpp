#include "camera.h"

dmcr::Ray dmcr::Camera::ray(float /* x */, float /* y */) const
{
    /* 
     * TODO
     * Implement me properly! (see issue #10)
     */
    return Ray(m_position, m_look_at - m_position);
}

