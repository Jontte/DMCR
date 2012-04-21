/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_RAYCASTRESULT_H
#define DMCR_RAYCASTRESULT_H

#include "vector.h"

namespace dmcr {

// Forward declaration to prevent circular dependency
class SceneObject;
typedef SceneObject* SceneObjectPtr;

/*!
 * \brief This class stores the result of a raycast
 * 
 * The RaycastResult class stores the intersection point and a pointer to the 
 * object with which the ray collided. It is used to pass data from a Scene
 * object to a Renderer object.
 */
class RaycastResult
{
public:
    RaycastResult() : m_object(nullptr) { }

    /*!
     * \brief Set pointer to collided object
     * \param object Pointer to object with which the ray collided
     */
    void setObject(dmcr::SceneObjectPtr object) { m_object = object; }
    /*!
     * \brief Get pointer to collided object
     * \return Pointer to object with which the ray collided
     */
    dmcr::SceneObjectPtr object() const { return m_object; }

    /*!
     * \brief Set intersection point between ray and object
     * \param intersection_point Intersection point between ray and object
     */
    void setIntersectionPoint(const dmcr::Vector3f& intersection_point) {
        m_intersection_point = intersection_point;
    }
    /*!
     * \brief Get intersection point between ray and object
     * \return Intersection point between ray and object
     */
    dmcr::Vector3f intersectionPoint() const { return m_intersection_point; }
    
    void setNormal(const dmcr::Vector3f& normal) { m_normal = normal; }
    dmcr::Vector3f normal() const { return m_normal; }
    
    dmcr::Vector3f uv() const { return m_uv; }
    void setUV(const dmcr::Vector3f& uv) { m_uv = uv; }

private:
    dmcr::SceneObjectPtr m_object;
    dmcr::Vector3f m_intersection_point;
    dmcr::Vector3f m_normal;
    dmcr::Vector3f m_uv;
};

} // namespace dmcr

#endif // DMCR_RAYCASTRESULT_H
