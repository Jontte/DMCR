#include "camera.h"

dmcr::Ray dmcr::Camera::ray(float x, float y) const
{
    /*
     * TODO
     * Implement me properly! (see issue #10)
     *
     *Pseudocode here:
     at = normalize(self.look_at)
     up = vector3(???)
     horizontal = normalize(cross(at,up))
     vertical       = cross(horizontal,at)
     H = tan(0.5 * FOV)
     horizontal = H * horizontal
     vertical = (1 / ASPECT) * H * vertical
     */
    at = (m_look_at - m_position).normalized();
    up = dmcr::Vector3f(0,1,0);
    horizontal = at.cross(up).normalized();
    vertical = horizontal.cross(at);
    H = tan(0.5 * m_fov * 180/ pi);
    horizontal = H * horizontal;
    vertical = (1 / m_aspect) * H * vertical;
    return Ray(m_position, at + (2*x-1)*horizontal + (2*y-1)*vertical);
    //return Ray(m_position, m_look_at - m_position); vanha
}
