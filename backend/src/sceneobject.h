/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>
#include <string>
#include "vector.h"
#include "raycastresult.h"

namespace dmcr {

struct IntersectionResult
{
    bool intersects;
    double t;
    dmcr::Vector3f normal;
};

class AABB;
class Ray;

/*!
 * \brief Describes an object in a scene
 * 
 * The SceneObject is an abstract base class from which all renderable objects
 * are to be inherited. A SceneObject has a defined position in 3D space.
 */
class SceneObject
{
public:
    SceneObject() : m_color({0.0f,0.0f,0.0f}), m_light(false) { }

    /*!
     * \brief Set object's position
     * \param position New position
     */
    void setPosition(dmcr::Vector3f position) { m_position = position; }
    /*!
     * \brief Get object's position
     * \return Object's position
     */
    dmcr::Vector3f position() const { return m_position; }
   /*!
    * \brief Set object's color'
    * \param color New color
    */
    void setColor(dmcr::Color color) { m_color = color; }
   /*!
    * \brief Get object's color'
    * \return Object's color
    */
    dmcr::Color color() const { return m_color; }
    
    void setLight(bool light) { m_light = light; }
    bool light() const { return m_light; }

    /*!
     * \brief Get a string describing the object's typedef
     * \return String describing object's type
     * 
     * This is mainly for debugging purposes.
     */
    virtual std::string type() const = 0;
    /*!
     * \brief Get object's axis aligned bounding box
     * \return Object's axis aligned bounding box
     */
    virtual dmcr::AABB aabb() const = 0;

    /*!
     * \brief Test intersection between object and ray
     * \param ray Ray to be tested
     * \return If the ray intersects, a positive floating point value
     * corresponding to the distance between the ray origin and the nearest
     * intersection point. If the ray does not intersect, a negative floating
     * point value.
     */
    virtual IntersectionResult intersects(const dmcr::Ray& ray) const = 0;

private:
    dmcr::Vector3f m_position;
    dmcr::Color m_color;
    bool m_light;
};

typedef SceneObject* SceneObjectPtr;

}

#endif // SCENEOBJECT_H
