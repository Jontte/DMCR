/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef KDTREESCENE_H
#define KDTREESCENE_H

#include "vector.h"
#include "sceneobject.h"
#include "scene.h"
#include <list>
#include "unique_ptr"

namespace dmcr {

class KDTreeScene : public dmcr::Scene
{
public:
    KDTreeScene();
    ~KDTreeScene();

    void beginAddObjects();
    void endAddObjects();

    void addObject(std::unique_ptr<dmcr::SceneObject> object);
    dmcr::RaycastResult shootRay(const dmcr::Ray& ray) const;

    std::list<SceneObjectPtr> intersectionCandidates(
            const dmcr::Ray& ray) const;

private:
    std::vector<std::unique_ptr<dmcr::SceneObject>> m_objects;

    struct impl;
    impl* m;
};

}

#endif // KDTREESCENE_H
