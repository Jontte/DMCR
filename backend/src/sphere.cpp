#include "sphere.h"

dmcr::AABB dmcr::Sphere::aabb() const
{
    return dmcr::AABB::
            fromCenterAndExtents(m_position,
                                 dmcr::Vector3f(m_radius, m_radius, m_radius));
}

double dmcr::Sphere::intersects(const dmcr::Ray &/*ray*/) const
{
    /* TODO: Implement me! */
    return -1.0;
}
