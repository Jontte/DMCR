/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_AABB_H
#define DMCR_AABB_H

#include "vector.h"
#include "ray.h"
#include "sceneobject.h"

namespace dmcr {

/*!
 * \brief Axis aligned bounding box
 * 
 * This class describes an axis aligned bounding box. According to
 * Wikipedia: "The axis-aligned minimum bounding box for a given point set is
 * its minimum bounding box subject to the constraint that the edges of the box
 * are parallel to the (Cartesian) coordinate axes. It is simply the Cartesian
 * product of N intervals each of which is defined by the minimal and maximal
 * value of the corresponding coordinate for the points in S."
 * 
 * An axis aligned bounding box is characterized by its minimum and maximum
 * corners, i.e. the corners for which the cartesian coordinates get their
 * minimum/maximum values.
 */
class AABB
{
public:
    AABB() { }
    /*!
     * \brief Construct an AABB using minimum and maximum corners
     * \param min Minimum corner coordinates
     * \param max Maximum corner coordinates
     */
    AABB(const dmcr::Vector3f& min, const dmcr::Vector3f& max);

    /*!
     * \brief Get minimum corner coordinates
     * \return Minimum corner coordinates
     */
    dmcr::Vector3f min() const { return m_min; }
    /*!
     * \brief Get maximum corner coordinates
     * \return Maximum corner coordinates
     */
    dmcr::Vector3f max() const { return m_max; }
    /*!
     * \brief Get center point
     * \return Center point
     */
    dmcr::Vector3f center() const { return (m_max + m_min) / 2.0; }
    /*!
     * \brief Get extents
     * \return Extents
     * 
     * This function returns a vector describing the size of the AABB. It is
     * the vector from the minimum corner to the maximum corner.
     */
    dmcr::Vector3f extents() const { return m_max - m_min; }

    /*!
     * \brief Set minimum corner coordinates
     * \param min Minimum corner coordinates
     */
    void setMin(const dmcr::Vector3f& min) { m_min = min; }
    /*!
     * \brief Set maximum corner coordinates
     * \param min Maximum corner coordinates
     */
    void setMax(const dmcr::Vector3f& max) { m_max = max; }

    /*!
     * \brief Test intersection with another AABB
     * \return True, if the AABBs are overlapped. False, otherwise.
     */
    bool intersects(const AABB& aabb) const;
    /*!
     * \brief Test intersection with a ray
     * \return If the ray intersects, a positive floating point value
     * corresponding to the distance between the ray origin and the intersection
     * point. If the ray does not intersect, a negative floating point value.
     */
    dmcr::IntersectionResult intersects(const dmcr::Ray& ray) const;
    
    /*!
     * \brief Construct an AABB using center and extents values
     * \param center AABB center
     * \param extents AABB extents
     * \return AABB corresponding to the giving center and extents values
     */
    static dmcr::AABB fromCenterAndExtents(const dmcr::Vector3f& center,
                                           const dmcr::Vector3f& extents);
    
    static dmcr::AABB fromOppositeCorners(const dmcr::Vector3f& corner1,
                                          const dmcr::Vector3f& corner2);
    /*!
     * \brief Construct a minimal AABB that contains the given list of AABBs
     * \param begin iterator to the beginning of the range
     * \param end iterator to the end of the range
     * \return minimal AABB that contains the given AABBs
     */
    template <class Iter>
    static dmcr::AABB fromRange(Iter begin, Iter end)
    {
        if( begin == end )
            return dmcr::AABB();
        
        dmcr::Vector3f min = begin->min();
        dmcr::Vector3f max = begin->max();

        for (;begin != end; begin++) {
            
            const dmcr::AABB& bb = *begin;

            if (bb.min().x() < min.x())
                min.setX(bb.min().x());
            if (bb.min().y() < min.y())
                min.setY(bb.min().y());
            if (bb.min().z() < min.z())
                min.setZ(bb.min().z());
            if (bb.max().x() > max.x())
                max.setX(bb.max().x());
            if (bb.max().y() > max.y())
                max.setY(bb.max().y());
            if (bb.max().z() > max.z())
                max.setZ(bb.max().z());
        }
        return dmcr::AABB(min, max);
    }

    bool operator==(const AABB& other) const
    {
        return min() == other.min() && max() == other.max();
    }
private:
    dmcr::Vector3f m_min, m_max;
};

} // namespace dmcr

#endif // DMCR_AABB_H
