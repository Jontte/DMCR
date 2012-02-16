#include <iostream>
#include <vector>
#include <string>
#include "vector.h"
#include "socket.h"
#include "dummyscene.h"
#include "sharedstream.h"
#include "renderer.h"
#include <csignal>

int main(int argc, char * argv[])
{
    // Parse command line parameters to a vector of strings
    std::vector<std::string> args(argv, argv+argc);

    try {
        // SharedStream tests
        dmcr::SharedStream<int> queue;
        queue.push(10);
        int kissa = 20;
        queue.push(kissa);

        kissa = queue.pull();
        assert(kissa == 10);
        assert(queue.pull() == 20);

        queue.pull(std::chrono::seconds(1));
    }
    catch(dmcr::StreamTimeoutException&)
    {
        // don't care
    }
    
    signal(SIGPIPE, SIG_IGN);

    std::cout << "Hello, World!" << std::endl;

    dmcr::ScenePtr scene(new dmcr::DummyScene);
    try {
        scene->loadFromFile("assets/scene.json");
        ((dmcr::DummyScene*)scene.get())->debugPrint();
    } catch (const dmcr::SceneException& e) {
        std::cout << "SceneException: " << e.what() << std::endl;
    }
        
    dmcr::Renderer renderer(scene);
    dmcr::RenderResultPtr result = renderer.render(640, 480);
    result->saveImage("test.ppm");

    if (args.size() > 1) {
        dmcr::Socket s(args[1], 9393);

        try {
            s.connect();
            s.run();
        } catch (const dmcr::SocketException& e) {
            std::cout << "SocketException: " << e.what() << std::endl;
        }
    }

    return 0;
}

