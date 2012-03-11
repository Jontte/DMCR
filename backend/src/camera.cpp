#include "camera.h"

dmcr::Ray dmcr::Camera::ray(float /* x */, float /* y */) const
{
    /*
     * TODO
     * Implement me properly! (see issue #10)
     *
     *Pseudocode here:
     at = normalize(self.look_at)
     up = vector3(???)
     horizontal = normalize(cross(at,up))
     vertical 	= cross(horizontal,at)
     H = tan(0.5 * FOV)
     horizontal = H * horizontal
     v = (1 / ASPECT) * H * vertical
     return Ray(position, (2x-1)*horizontal + (2y-1)*vertical - position)
     */
    return Ray(m_position, m_look_at - m_position);
}
