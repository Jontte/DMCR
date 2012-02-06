#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>
#include <string>
#include "vector.h"

namespace dmcr {

class SceneObject
{
public:
    SceneObject();

    void setPosition(Vector3f position) { m_position = position; }
    Vector3f position() const { return m_position; }

    virtual std::string type() const = 0;

protected:
    Vector3f m_position;
};

class Sphere : public dmcr::SceneObject
{
public:
    Sphere();

    void setRadius(float radius) { m_radius = radius; }
    float radius() const { return m_radius; }

    virtual std::string type() const { return "sphere"; }

protected:
    float m_radius;
};

class Box : public dmcr::SceneObject
{
public:
    Box();

    void setExtents(Vector3f extents) { m_extents = extents; }
    Vector3f extents() const { return m_extents; }

    virtual std::string type() const { return "box"; }

protected:
    Vector3f m_extents;
};

typedef std::shared_ptr<SceneObject> SceneObjectPtr;

}

#endif // SCENEOBJECT_H
