/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <csignal>
#include <iostream>
#include <stdexcept>
#include <string>

#include "backendapplication.h"
#include "settings.h"

int main(int argc, char * argv[])
{
    dmcr::Settings::get().loadArgs(argc, argv);

    signal(SIGPIPE, SIG_IGN);

    std::string host = dmcr::Settings::get().readString("host", "");
    int port = dmcr::Settings::get().readInt("port", 9393);
    if (host.length() > 0) {
        dmcr::BackendApplication app;

        try {
            app.connect(host, port);
            app.start();
        } catch (const std::runtime_error& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Usage: " << argv[0] << " --host <address> [--port 9393]" << std::endl;
        std::cout << "Connects to DMCR frontend located at <address> <port>" << std::endl;
        return 1;
    }

    return 0;
}

