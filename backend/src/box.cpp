/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "box.h"

dmcr::AABB dmcr::Box::aabb() const
{
    return dmcr::AABB::fromCenterAndExtents(m_position, m_extents);
}

double dmcr::Box::intersects(const dmcr::Ray &ray) const
{
    return aabb().intersects(ray);
}
