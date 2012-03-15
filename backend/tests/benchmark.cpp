#include "../src/taskmanager.h"
#include "../src/task.h"
#include "../src/scene.h"
#include "../src/octreescene.h"
#include "../src/itaskprovider.h"
#include "../src/box.h"
#include <unistd.h>
#include <string>
#include <iostream>

class BenchmarkTaskProvider : public dmcr::ITaskProvider {
public:
    virtual void onTaskCompleted(uint32_t /*task_id*/, 
                                 const dmcr::RenderResultPtr& /*result*/)
    {
        std::cout << "Benchmark done." << std::endl;
        exit(0);
    }
};

int main() {
    dmcr::SceneFactory factory = [](const std::string& /*str*/) {
        auto scene = std::make_shared<dmcr::OctreeScene>();
        scene->beginAddObjects();

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                auto a = dmcr::make_unique<dmcr::Box>();
                a->setPosition(dmcr::Vector3f(2 * i, 2 * j, 0));
                a->setExtents(dmcr::Vector3f(1, 1, 1));
                scene->addObject(std::move(a));
            }
        }

        scene->endAddObjects();

        std::cout << "Scene constructed." << std::endl;

        return scene;
    };
    
    dmcr::TaskManager manager(factory);
    
    BenchmarkTaskProvider provider;
    
    manager.onNewTask(&provider, 1, 640, 480, 1, "");
    
    for (;;)
        sleep(1);
}
