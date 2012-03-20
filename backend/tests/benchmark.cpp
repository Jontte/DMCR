/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "../src/taskmanager.h"
#include "../src/task.h"
#include "../src/scene.h"
#include "../src/kdtreescene.h"
#include "../src/octreescene.h"
#include "../src/dummyscene.h"
#include "../src/itaskprovider.h"
#include "../src/box.h"
#include "../src/sphere.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <cmath>

class BenchmarkTaskProvider : public dmcr::ITaskProvider {
public:
    virtual void onTaskCompleted(uint32_t /*task_id*/, 
                                 const dmcr::RenderResultPtr& result)
    {
        std::cout << "Benchmark done." << std::endl;
        result->saveImage("benchmark.ppm");
        exit(0);
    }
};

int main() {
    dmcr::SceneFactory factory = [](const std::string& /*str*/) {
        auto scene = std::make_shared<dmcr::KDTreeScene>();
        auto& c = scene->camera();
        c.setPosition(dmcr::Vector3f(9, 3, 5));
        c.setFov(M_PI / 2);
        c.setAspect(640.0f / 480.0f);
        c.setLookAt(dmcr::Vector3f(6, 6, 0));
       
        scene->beginAddObjects();
        
        auto b = dmcr::make_unique<dmcr::Box>();
        b->setPosition(dmcr::Vector3f(5, 0, 0));
        b->setExtents(dmcr::Vector3f(10, 0.2, 10));
        b->setColor({1, 1, 1});
        b->setLight(1);
        scene->addObject(std::move(b));
        b = dmcr::make_unique<dmcr::Box>();
        b->setPosition(dmcr::Vector3f(5, 11, 0));
        b->setExtents(dmcr::Vector3f(10, 0.2, 10));
        b->setColor({1, 1, 1});
        scene->addObject(std::move(b));

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                /*
                auto a = dmcr::make_unique<dmcr::Box>();
                a->setPosition(dmcr::Vector3f(i + 0.5, j + 0.5, 0.5));
                a->setExtents(dmcr::Vector3f(0.5, 0.5, 0.5));
                a->setColor({1.0, 1.0, 1.0});
                //a->setLight(i % 3 == 0 && j % 3 == 0);
                scene->addObject(std::move(a));
                */

                auto a = dmcr::make_unique<dmcr::Sphere>();
                a->setPosition(dmcr::Vector3f(i + 0.5, j + 0.5, 0.5));
                a->setRadius(0.2);
                a->setColor({1.0, 1.0, 1.0});
                scene->addObject(std::move(a));
            }
        }

        scene->endAddObjects();

        std::cout << "Scene constructed." << std::endl;

        return scene;
    };
    
    dmcr::TaskManager manager(factory);
    
    BenchmarkTaskProvider provider;
    
    manager.onNewTask(&provider, 1, 640, 480, 25, "");
    
    for (;;)
        sleep(1);
}
