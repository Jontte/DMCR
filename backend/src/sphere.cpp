/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <algorithm>
#include <cmath>

#include "ray.h"
#include "sphere.h"
#include "vector.h"
#include "random.h"

dmcr::AABB dmcr::Sphere::aabb() const
{
    return dmcr::AABB::
            fromCenterAndExtents(dmcr::SceneObject::position(),
                                 dmcr::Vector3f(m_radius * 2, 
                                                m_radius * 2,
                                                m_radius * 2));
}

dmcr::IntersectionResult dmcr::Sphere::intersects(const dmcr::Ray & ray)
const
{
    dmcr::Vector3f ray_origin = ray.origin() - position();
    
    double a = ray.direction().dot(ray.direction());
    double b = 2 * ray.direction().dot(ray_origin);
    double c = ray_origin.dot(ray_origin) - m_radius * m_radius;

    double dcrim = b * b - 4 * a * c;
    if (dcrim < 0.0) {
        dmcr::IntersectionResult result;
        result.intersects = false;
        return result;
    }

    double a2inv = 1.0 / (2.0 * a);
    double t0 = (-b + sqrt(dcrim)) * a2inv;
    double t1 = (-b - sqrt(dcrim)) * a2inv;

    dmcr::IntersectionResult result;
    result.intersects = true;
    result.t = std::min(t0, t1);
    result.normal = (ray.origin() + result.t * ray.direction() - position()) /
                    m_radius;
    
    return result;
}

dmcr::Vector3f dmcr::Sphere::randomPoint() const
{
    static dmcr::RNG rng;
    return position() + rng.random_vector() * m_radius;
}
