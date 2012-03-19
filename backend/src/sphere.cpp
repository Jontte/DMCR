/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "sphere.h"

dmcr::AABB dmcr::Sphere::aabb() const
{
    return dmcr::AABB::
            fromCenterAndExtents(dmcr::SceneObject::position(),
                                 dmcr::Vector3f(m_radius, m_radius, m_radius));
}

double dmcr::Sphere::intersects(const dmcr::Ray &/*ray*/) const
{
    /* TODO: Implement me! */
    return -1.0;
}
