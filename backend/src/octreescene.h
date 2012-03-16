/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef OCTREESCENE_H
#define OCTREESCENE_H

#include "vector.h"
#include "sceneobject.h"
#include "scene.h"
#include "octree.h"
#include <list>
#include "unique_ptr"

namespace dmcr {

class OctreeScene : public dmcr::Scene
{
public:
    OctreeScene();

    void beginAddObjects();
    void endAddObjects();

    void addObject(std::unique_ptr<dmcr::SceneObject> object);
    dmcr::RaycastResult shootRay(const dmcr::Ray& ray) const;

    std::list<SceneObjectPtr> intersectionCandidates(
            const dmcr::Ray& ray) const;

private:
    Octree m_octree;
    std::vector<std::unique_ptr<dmcr::SceneObject>> m_objects;
};

}

#endif // OCTREESCENE_H
