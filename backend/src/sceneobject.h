#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>
#include "vector.h"

namespace dmcr {

class SceneObject
{
public:
    SceneObject();

    void setPosition(Vector3 position) { m_position = position; }
    Vector3 position() const { return m_position; }

    virtual std::string type() const = 0;

protected:
    Vector3 m_position;
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

    virtual std::string type() const { return "box"; }
};

typedef std::shared_ptr<SceneObject> SceneObjectPtr;

}

#endif // SCENEOBJECT_H
