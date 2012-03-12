#include "aabb.h"
#include <cmath>

#define EPSILON 0.00000000000001

dmcr::AABB::AABB(const dmcr::Vector3f &min, const dmcr::Vector3f &max) :
    m_min(min), m_max(max)
{
}

bool dmcr::AABB::intersects(const dmcr::AABB &aabb) const
{
    if (aabb.min().x() > m_max.x())
        return false;
    if (aabb.min().y() > m_max.y())
        return false;
    if (aabb.min().z() > m_max.z())
        return false;
    if (aabb.max().x() < m_min.x())
        return false;
    if (aabb.max().y() < m_min.y())
        return false;
    if (aabb.max().z() < m_min.z())
        return false;
    return true;
}

bool dmcr::AABB::intersects(const dmcr::Ray &ray) const
{
    for (int i = 0; i < 3; ++i) {
        if (fabs(ray.direction()[i]) < EPSILON &&
                (ray.origin()[i] < m_min[i] || ray.origin()[i] > m_max[i]))
            return false;
    }

    dmcr::Vector3f t1 = (m_min - ray.origin()) / ray.direction();
    dmcr::Vector3f t2 = (m_max - ray.origin()) / ray.direction();

    for (int i = 0; i < 3; ++i) {
        if (t1[i] > t2[i]) {
            float tmp = t1[i];
            t1[i] = t2[i];
            t2[i] = tmp;
        }
    }

    float tmin = t1[0];
    float tmax = t2[0];

    for (int i = 1; i < 3; ++i) {
        if (t1[i] > tmin)
            tmin = t1[i];
        if (t2[i] < tmax)
            tmax = t2[i];
    }

    if (tmin > tmax)
        return false;

    if (tmax < 0)
        return false;

    return true;
}

dmcr::AABB dmcr::AABB::fromCenterAndExtents(const dmcr::Vector3f &center,
                                            const dmcr::Vector3f &extents)
{
    return dmcr::AABB(center - extents, center + extents);
}
