#ifndef DMCR_BOX_H
#define DMCR_BOX_H

#include "sceneobject.h"
#include "vector.h"
#include "aabb.h"

namespace dmcr {

class Box : public dmcr::SceneObject
{
public:
    Box() { }

    void setExtents(dmcr::Vector3f extents) { m_extents = extents; }
    dmcr::Vector3f extents() const { return m_extents; }

    virtual std::string type() const { return "box"; }
    virtual dmcr::AABB aabb() const;

protected:
    dmcr::Vector3f m_extents;
};

} // namespace dmcr

#endif // DMCR_BOX_H
