#include "ray.h"

dmcr::Ray::Ray(const dmcr::Vector3f& origin, const dmcr::Vector3f& dir) :
    m_origin(origin),
    m_direction(dir)
{
}
