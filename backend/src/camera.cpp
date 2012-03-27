/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "camera.h"
#include <cmath>

void dmcr::Camera::calculateCachedValues(){
        m_at = (m_look_at - m_position).normalized();
        dmcr::Vector3f up(0,1,0);
        m_horizontal = at.cross(up);
        m_vertical = at.cross(m_horizontal);
        double H = tan(0.5 * m_fov);
        m_horizontal = m_horizontal * H;
        m_vertical = m_vertical * (1 / m_aspect) * H;
    }

dmcr::Ray dmcr::Camera::ray(double x, double y) const
{
    return Ray(m_position, (m_at + m_horizontal * (2*x-1) + m_vertical * (2*y-1)).normalized());
}
