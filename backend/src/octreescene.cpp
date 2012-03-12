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

dmcr::RaycastResult dmcr::OctreeScene::shootRay(const dmcr::Ray &ray) const
{
    dmcr::RaycastResult result;
    result.setIntersectionPoint(ray.origin());

    std::list<SceneObjectPtr> objects = m_octree.getObjects(ray);
    if (objects.size() > 0)
        result.setObject(objects.front());
    else
        result.setObject(nullptr);
    return result;
}

