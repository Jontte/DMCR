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
    void addObject(dmcr::SceneObjectPtr &object);
    void debugPrint() const;

private:
    std::vector<dmcr::SceneObjectPtr> m_objects;
};

}

#endif // DUMMYSCENE_H
