/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "../src/settings.h"
#include <stdexcept>
#include <iostream>

void test_settings() {
    auto& s = dmcr::Settings::get();
    s.load("test.set");

    std::cout << s.readInt("test",-1);

    if (s.readInt("test",-1) == -1)
        throw std::runtime_error("Key test was not found or value was -1");
}

