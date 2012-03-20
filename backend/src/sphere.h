/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_SPHERE_H
#define DMCR_SPHERE_H

#include "sceneobject.h"
#include "vector.h"
#include "aabb.h"
#include "raycastresult.h"

namespace dmcr {

class Sphere : public dmcr::SceneObject
{
public:
    Sphere() : m_radius(0.0f) { }

    void setRadius(float radius) { m_radius = radius; }
    float radius() const { return m_radius; }

    virtual std::string type() const { return "sphere"; }
    virtual dmcr::AABB aabb() const;

    dmcr::IntersectionResult intersects(const Ray &ray) const;

protected:
    float m_radius;
};

} // namespace dmcr

#endif // DMCR_SPHERE_H
