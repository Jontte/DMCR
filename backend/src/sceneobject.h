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

    void setPosition(dmcr::Vector3f position) { m_position = position; }
    dmcr::Vector3f position() const { return m_position; }

    virtual std::string type() const = 0;

protected:
    dmcr::Vector3f m_position;
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

    void setExtents(dmcr::Vector3f extents) { m_extents = extents; }
    dmcr::Vector3f extents() const { return m_extents; }

    virtual std::string type() const { return "box"; }

protected:
    dmcr::Vector3f m_extents;
};

typedef SceneObject* SceneObjectPtr;

}

#endif // SCENEOBJECT_H
