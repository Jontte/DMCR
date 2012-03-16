/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "dummyscene.h"
#include <iostream>

dmcr::DummyScene::DummyScene()
{
}

void dmcr::DummyScene::addObject(std::unique_ptr<dmcr::SceneObject> object)
{
    m_objects.push_back(std::move(object));
}

std::list<dmcr::SceneObjectPtr> dmcr::DummyScene::intersectionCandidates(
        const dmcr::Ray& /* ray */) const
{
    std::list<dmcr::SceneObjectPtr> objects;
    for (auto& obj : m_objects)
        objects.push_back(obj.get());
    return objects;
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
