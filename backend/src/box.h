/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_BOX_H
#define DMCR_BOX_H

#include <string>

#include "aabb.h"
#include "sceneobject.h"
#include "vector.h"

namespace dmcr {

class Ray;

class Box : public dmcr::SceneObject
{
public:
    Box() { }

    /*!
     * \brief Set extents
     * \param extents Box extents
     * 
     * The box's extents is defined as the vector between two opposite corners.
     */
    void setExtents(dmcr::Vector3f extents) { m_extents = extents; }
    /*!
     * \brief Get extents
     * \return Box extents
     * 
     * This function returns a vector describing the size of the box. It is
     * the vector from the center point to the maximum corner.
     */
    dmcr::Vector3f extents() const { return m_extents; }

    virtual std::string type() const { return "box"; }
    virtual dmcr::AABB aabb() const;

    dmcr::IntersectionResult intersects(const Ray &ray) const;

    dmcr::Vector3f randomPoint() const;

protected:
    dmcr::Vector3f m_extents;
};

} // namespace dmcr

#endif // DMCR_BOX_H
