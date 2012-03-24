/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_OCTREE_H
#define DMCR_OCTREE_H

#include <list>

#include "aabb.h"
#include "ray.h"
#include "sceneobject.h"
#include "unique_ptr"

namespace dmcr {

class Ray;

#define MAX_OBJECTS 5
#define MAX_DEPTH 10

class Octree
{
public:
    Octree();

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
