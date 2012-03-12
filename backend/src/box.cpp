#include "box.h"

dmcr::AABB dmcr::Box::aabb() const
{
    return dmcr::AABB::fromCenterAndExtents(m_position, m_extents);
}
