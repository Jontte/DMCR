/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "camera.h"
#include <cmath>

dmcr::Ray dmcr::Camera::ray(double x, double y) const
{
    dmcr::Vector3f at = (m_look_at - m_position).normalized();
    dmcr::Vector3f up(0,1,0);
    dmcr::Vector3f horizontal = at.cross(up);
    dmcr::Vector3f vertical = at.cross(horizontal);
    double H = tan(0.5 * m_fov);
    horizontal = horizontal * H;
    vertical = vertical * (1 / m_aspect) * H;
    
    return Ray(m_position, (at + horizontal * (2*x-1) + vertical *
(2*y-1)).normalized());
}
