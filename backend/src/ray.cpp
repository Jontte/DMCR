/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "ray.h"

dmcr::Ray::Ray(const dmcr::Vector3f& origin, const dmcr::Vector3f& dir) :
    m_origin(origin),
    m_direction(dir)
{
}
