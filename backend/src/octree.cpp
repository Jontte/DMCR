/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <vector>
#include <algorithm>
#include "octree.h"

dmcr::Vector3f extentPermutations[8] =
{ dmcr::Vector3f(1, 1, 1), dmcr::Vector3f(1, 1, -1),
  dmcr::Vector3f(1, -1, 1), dmcr::Vector3f(1, -1, -1),
  dmcr::Vector3f(-1, 1, 1), dmcr::Vector3f(-1, 1, -1),
  dmcr::Vector3f(-1, -1, 1), dmcr::Vector3f(-1, -1, -1) };

dmcr::Octree::Octree() :
    m_leaf(false)
{
}

void dmcr::Octree::build(std::list<dmcr::SceneObjectPtr> objects,
                         int depth = 0)
{
    m_objects.clear();
    
    if (depth == 0) {
        std::vector<AABB> aabbs;
        aabbs.resize(objects.size());
        std::transform(objects.begin(), objects.end(), aabbs.begin(),
            [](dmcr::SceneObjectPtr p){return p->aabb();
        });

        m_aabb = dmcr::AABB::fromRange(aabbs.begin(), aabbs.end());
    }

    std::list<dmcr::SceneObjectPtr> inside;

    for (auto obj : objects) {
        if (obj->aabb().intersects(m_aabb))
            inside.push_back(obj);
    }

    if (inside.empty()) {
        m_leaf = true;
        return;
    }
    else if (inside.size() <= MAX_OBJECTS || depth == MAX_DEPTH) {
        m_objects.insert(m_objects.begin(), inside.begin(), inside.end());
        m_leaf = true;
        return;
    } else {
        m_leaf = false;

        dmcr::Vector3f center = m_aabb.center();
        dmcr::Vector3f half_extents = m_aabb.extents() / 2.0;

        for (int i = 0; i < 8; ++i) {
            m_children[i] = dmcr::make_unique<dmcr::Octree>();

            m_children[i]->setAabb(dmcr::AABB::fromOppositeCorners(center,
                                              center + half_extents *
                                              extentPermutations[i]));

            m_children[i]->build(inside, depth + 1);
        }
    }
}

std::list<dmcr::SceneObjectPtr>
    dmcr::Octree::getObjects(const dmcr::Ray &ray) const
{
    std::list<dmcr::SceneObjectPtr> result;

    if (!m_aabb.intersectsBool(ray))
        return result;

    if (m_leaf) {
        result.insert(result.end(), m_objects.begin(), m_objects.end());
    } else {
        for (int i = 0; i < 8; ++i) {
            const std::list<dmcr::SceneObjectPtr>& objects =
                    m_children[i]->getObjects(ray);
            result.insert(result.end(), objects.begin(), objects.end());
        }
    }

    return result;
}
