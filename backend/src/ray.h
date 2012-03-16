/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef RAY_H
#define RAY_H

#include "vector.h"

namespace dmcr {

class Ray
{
public:
    Ray() { }
    Ray(const dmcr::Vector3f& origin, const dmcr::Vector3f& dir);
    
    void setOrigin(const dmcr::Vector3f& origin) { m_origin = origin; }
    dmcr::Vector3f origin() const { return m_origin; }
    
    void setDirection(const dmcr::Vector3f& dir) { m_direction = dir; }
    dmcr::Vector3f direction() const { return m_direction; }

protected:
    dmcr::Vector3f m_origin;
    dmcr::Vector3f m_direction;
};

}

#endif // RAY_H
