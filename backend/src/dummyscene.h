#ifndef DUMMYSCENE_H
#define DUMMYSCENE_H

#include "scene.h"
#include "sceneobject.h"
#include <vector>

namespace dmcr {

class DummyScene : public dmcr::Scene
{
public:
    DummyScene();
    void addObject(std::unique_ptr<dmcr::SceneObject> object);
    dmcr::RaycastResult shootRay(const dmcr::Ray& ray) const;
    void debugPrint() const;

private:
    std::vector<std::unique_ptr<dmcr::SceneObject>> m_objects;
};

}

#endif // DUMMYSCENE_H
