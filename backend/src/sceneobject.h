#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>
#include <string>
#include "vector.h"
#include "ray.h"
#include "aabb.h"
#include "raycastresult.h"

namespace dmcr {

class SceneObject
{
public:
    SceneObject() { }

    void setPosition(dmcr::Vector3f position) { m_position = position; }
    dmcr::Vector3f position() const { return m_position; }

    virtual std::string type() const = 0;
    virtual dmcr::AABB aabb() const = 0;

    virtual double intersects(const dmcr::Ray& ray) const = 0;

protected:
    dmcr::Vector3f m_position;
};

typedef SceneObject* SceneObjectPtr;

}

#endif // SCENEOBJECT_H
