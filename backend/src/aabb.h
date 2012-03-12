#ifndef DMCR_AABB_H
#define DMCR_AABB_H

#include "vector.h"
#include "ray.h"

namespace dmcr {

class AABB
{
public:
    AABB() { }
    AABB(const dmcr::Vector3f& min, const dmcr::Vector3f& max);

    dmcr::Vector3f min() const { return m_min; }
    dmcr::Vector3f max() const { return m_max; }
    dmcr::Vector3f center() const { return (m_max + m_min) / 2.0; }
    dmcr::Vector3f extents() const { return (m_max - m_min) / 2.0; }

    void setMin(const dmcr::Vector3f& min) { m_min = min; }
    void setMax(const dmcr::Vector3f& max) { m_max = max; }

    bool intersects(const AABB& aabb) const;
    bool intersects(const dmcr::Ray& ray) const;

    static dmcr::AABB fromCenterAndExtents(const dmcr::Vector3f& center,
                                           const dmcr::Vector3f& extents);

private:
    dmcr::Vector3f m_min, m_max;
};

} // namespace dmcr

#endif // DMCR_AABB_H
