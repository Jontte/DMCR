/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_SPHERE_H
#define DMCR_SPHERE_H

#include <string>

#include "aabb.h"
#include "raycastresult.h"
#include "sceneobject.h"
#include "vector.h"

namespace dmcr {

class Ray;

class Sphere : public dmcr::SceneObject
{
public:
    Sphere() : m_radius(0.0) { }

    void setRadius(double radius) { m_radius = radius; }
    double radius() const { return m_radius; }
    
    dmcr::Vector3f randomPoint() const;

    virtual std::string type() const { return "sphere"; }
    virtual dmcr::AABB aabb() const;

    dmcr::IntersectionResult intersects(const Ray &ray) const;

protected:
    double m_radius;
};

} // namespace dmcr

#endif // DMCR_SPHERE_H
