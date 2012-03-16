/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "aabb.h"
#include <cmath>

#define EPSILON 0.000000001

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

// Test ray and AABB intersection using slabs method.
double dmcr::AABB::intersects(const dmcr::Ray &ray) const
{
    /* Check if the ray is parallel to a slab defined by parallel sides
     * of the AABB. If it is, check if its origin is outside the slab.
     */
    for (int i = 0; i < 3; ++i) {
        if (fabs(ray.direction()[i]) < EPSILON &&
                (ray.origin()[i] < m_min[i] || ray.origin()[i] > m_max[i]))
            return -1.0;
    }

    // Calculate parameter values for ray and slab intersection points
    dmcr::Vector3f t1 = (m_min - ray.origin()) / ray.direction();
    dmcr::Vector3f t2 = (m_max - ray.origin()) / ray.direction();

    // Assign minimum parameter values to t1
    for (int i = 0; i < 3; ++i) {
        if (t1[i] > t2[i]) {
            float tmp = t1[i];
            t1[i] = t2[i];
            t2[i] = tmp;
        }
    }
    
    /* Find the maximum of the minimum parameters and the minimum of the
     * maximum parameters.
     */

    float tmin = t1[0];
    float tmax = t2[0];

    for (int i = 1; i < 3; ++i) {
        if (t1[i] > tmin)
            tmin = t1[i];
        if (t2[i] < tmax)
            tmax = t2[i];
    }

    // The ray must enter all slabs before leaving any.
    if (tmin > tmax)
        return -1.0;

    // Check if the intersection point is on the correct side of the ray origin.
    if (tmax < 0)
        return -1.0;


    if (tmin < 0)
        return tmax;

    return tmin;
}

dmcr::AABB dmcr::AABB::fromCenterAndExtents(const dmcr::Vector3f &center,
                                            const dmcr::Vector3f &extents)
{
    return dmcr::AABB(center - extents, center + extents);
}
