/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <iostream>
#include <vector>
#include <string>
#include "vector.h"
#include "dummyscene.h"
#include "sharedstream.h"
#include "renderer.h"
#include "util.h"
#include "settings.h"
#include <csignal>

#include "backendapplication.h"

int main(int argc, char * argv[])
{
    dmcr::Settings::get().loadArgs(argc, argv);

    signal(SIGPIPE, SIG_IGN);

    if (args.size() > 1) {
        dmcr::BackendApplication app;

        int port = 9393;
        if( args.size() > 2)
        {
            port = dmcr::lexical_cast<int>(args[2]);
        }

        try {
            app.connect(args[1], port);
            app.start();
        } catch (const std::runtime_error& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Usage: " << args.at(0) << " <address> [port=9393]" << std::endl;
        std::cout << "Connects to DMCR frontend located at <address> <port>" << std::endl;
        return 1;
    }

    return 0;
}

