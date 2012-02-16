#include "dummyscene.h"
#include <iostream>

dmcr::DummyScene::DummyScene()
{
}

void dmcr::DummyScene::addObject(std::unique_ptr<dmcr::SceneObject> object)
{
    m_objects.push_back(std::move(object));
}

dmcr::RaycastResult dmcr::DummyScene::shootRay(const dmcr::Ray& ray) const
{
    RaycastResult result;
    result.setIntersectionPoint(ray.origin());
    result.setObject(m_objects.front().get());
    return result;
}

void dmcr::DummyScene::debugPrint() const
{
    for (const auto& object : m_objects) {   
        std::cout << "Hello, this is " << object->type() << std::endl;
        std::cout << object->position().x() << ", "
                  << object->position().y() << ", "
                  << object->position().z() << std::endl;
    }
}
