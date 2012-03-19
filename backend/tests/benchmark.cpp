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
        //auto scene = std::make_shared<dmcr::OctreeScene>();
        auto& c = scene->camera();
        c.setPosition(dmcr::Vector3f(0, 0, 10));
        c.setFov(M_PI / 2);
        c.setAspect(640.0f/480.0f);
        c.setLookAt(dmcr::Vector3f(0,0,0));
        
        scene->beginAddObjects();

        for (int i = 0; i < 1000; ++i) {
            for (int j = 0; j < 1000; ++j) {
                auto a = dmcr::make_unique<dmcr::Box>();
                a->setPosition(dmcr::Vector3f(i/20.0, j/20.0, 0));
                a->setExtents(dmcr::Vector3f(0.01, 0.01, 0.01));
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
