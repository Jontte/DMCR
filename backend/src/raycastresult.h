/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_RAYCASTRESULT_H
#define DMCR_RAYCASTRESULT_H

#include "vector.h"

namespace dmcr {

class SceneObject;
typedef SceneObject* SceneObjectPtr;

class RaycastResult
{
public:
    RaycastResult() : m_object(nullptr) { }

    void setObject(dmcr::SceneObjectPtr object) { m_object = object; }
    dmcr::SceneObjectPtr object() const { return m_object; }

    void setIntersectionPoint(const dmcr::Vector3f& intersection_point) {
        m_intersection_point = intersection_point;
    }
    dmcr::Vector3f intersectionPoint() const { return m_intersection_point; }

private:
    dmcr::SceneObjectPtr m_object;
    dmcr::Vector3f m_intersection_point;
};

} // namespace dmcr

#endif // DMCR_RAYCASTRESULT_H
