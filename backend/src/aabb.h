#ifndef DMCR_AABB_H
#define DMCR_AABB_H

#include "vector.h"
#include "ray.h"

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
     * the vector from the center point to the maximum corner.
     */
    dmcr::Vector3f extents() const { return (m_max - m_min) / 2.0; }

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
    double intersects(const dmcr::Ray& ray) const;

    /*!
     * \brief Construct an AABB using center and extents values
     * \param center AABB center
     * \param extents AABB extents
     * \return AABB corresponding to the giving center and extents values
     */
    static dmcr::AABB fromCenterAndExtents(const dmcr::Vector3f& center,
                                           const dmcr::Vector3f& extents);

private:
    dmcr::Vector3f m_min, m_max;
};

} // namespace dmcr

#endif // DMCR_AABB_H
