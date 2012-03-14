#include "octreescene.h"

dmcr::OctreeScene::OctreeScene()
{
}

void dmcr::OctreeScene::beginAddObjects()
{
}

void dmcr::OctreeScene::endAddObjects()
{
    std::list<SceneObjectPtr> objects;
    for (auto& i : m_objects)
        objects.push_back(i.get());
    m_octree.build(objects, 0);
}

void dmcr::OctreeScene::addObject(std::unique_ptr<dmcr::SceneObject> object)
{
    m_objects.push_back(std::move(object));
}

std::list<dmcr::SceneObjectPtr> dmcr::OctreeScene::intersectionCandidates(
        const dmcr::Ray &ray) const
{
    std::list<dmcr::SceneObjectPtr> objects = m_octree.getObjects(ray);
    return objects;
}


