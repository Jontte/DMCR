/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef OCTREESCENE_H
#define OCTREESCENE_H

#include <list>

#include "octree.h"
#include "raycastresult.h"
#include "scene.h"
#include "sceneobject.h"
#include "unique_ptr"
#include "vector.h"

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
class Ray;

}

#endif // OCTREESCENE_H
