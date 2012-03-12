#ifndef DMCR_OCTREE_H
#define DMCR_OCTREE_H

#include "aabb.h"
#include "sceneobject.h"
#include "ray.h"
#include <list>
#include "unique_ptr"

namespace dmcr {

#define MAX_OBJECTS 5
#define MAX_DEPTH 10

class Octree
{
public:
    Octree() { }

    void setAabb(const dmcr::AABB& aabb) { m_aabb = aabb; }
    void build(std::list<SceneObjectPtr> objects, int depth);

    std::list<dmcr::SceneObjectPtr> getObjects(const dmcr::Ray& ray) const;

private:
    dmcr::AABB m_aabb;

    std::unique_ptr<dmcr::Octree> m_children[8];

    std::list<dmcr::SceneObjectPtr> m_objects;

    bool m_leaf;
};

} // namespace dmcr

#endif // DMCR_OCTREE_H
