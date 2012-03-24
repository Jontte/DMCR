/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "box.h"

namespace dmcr {
class Ray;
}  // namespace dmcr

dmcr::AABB dmcr::Box::aabb() const
{
    return dmcr::AABB::fromCenterAndExtents(dmcr::SceneObject::position(), m_extents);
}

dmcr::IntersectionResult dmcr::Box::intersects(const dmcr::Ray &ray) const
{
    return aabb().intersects(ray);
}
