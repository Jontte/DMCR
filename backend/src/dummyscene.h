/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DUMMYSCENE_H
#define DUMMYSCENE_H

#include <list>
#include <vector>

#include "raycastresult.h"
#include "scene.h"
#include "sceneobject.h"

namespace dmcr {

class DummyScene : public dmcr::Scene
{
public:
    DummyScene();

    void beginAddObjects() { }
    void endAddObjects() { }
    
    void addObject(std::unique_ptr<dmcr::SceneObject> object);
    dmcr::RaycastResult shootRay(const dmcr::Ray& ray) const;
    void debugPrint() const;

    std::list<SceneObjectPtr> intersectionCandidates(
            const dmcr::Ray& ray) const;

private:
    std::vector<std::unique_ptr<dmcr::SceneObject>> m_objects;
};
class Ray;

}

#endif // DUMMYSCENE_H
