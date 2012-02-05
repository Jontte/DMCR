#include "dummyscene.h"
#include <iostream>

dmcr::DummyScene::DummyScene()
{
}

void dmcr::DummyScene::addObject(dmcr::SceneObjectPtr &object)
{
    m_objects.push_back(object);
}

void dmcr::DummyScene::debugPrint() const
{
    for (dmcr::SceneObjectPtr object : m_objects) {
        std::cout << "Hello, this is " << object->type() << std::endl;
        std::cout << object->position().x() << ", "
                  << object->position().y() << ", "
                  << object->position().z() << std::endl;
    }
}
