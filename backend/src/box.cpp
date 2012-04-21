/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "box.h"
#include "random.h"

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

dmcr::Vector3f dmcr::Box::randomPoint() const
{
    static dmcr::RNG rng;

    double areas[3];
    areas[0] = m_extents[1] * m_extents[2];
    areas[1] = m_extents[0] * m_extents[2];
    areas[2] = m_extents[0] * m_extents[1];
    double random = rng.random() * (areas[0] + areas[1] + areas[2]);
    dmcr::Vector3f result;
    if(random < areas[0]) {
        result = dmcr::Vector3f(m_extents[0] * 0.5,
                                m_extents[1] * (rng.random() - 0.5),
                                m_extents[2] * (rng.random() - 0.5));
    }
    else if(random < areas[1]) {
        result = dmcr::Vector3f(m_extents[0] * (rng.random() - 0.5),
                                m_extents[1] * 0.5,
                                m_extents[2] * (rng.random() - 0.5));
    }
    else {
        result = dmcr::Vector3f(m_extents[0] * (rng.random() - 0.5),
                                m_extents[1] * (rng.random() - 0.5),
                                m_extents[2] * 0.5);
    }
    return rng.random() < 0.5 ? position() + result : position() - result;
}
