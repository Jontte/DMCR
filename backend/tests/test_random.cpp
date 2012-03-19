/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <iostream>
#include <vector>
#include <cassert>
#include "../src/random.h"

void test_random()
{
    // Test seeding (potentially nondeterministic..., could fail in rare cases)
    {
        dmcr::RNG r1;
        dmcr::RNG r2;
        float f1 = r1.random();
        float f2 = r2.random();

        assert(f1 != f2);
    }
    // Test vector length
    {
        dmcr::RNG v1;
        assert(fabs(v1.random_vector().length() - 1.0) < 0.00001);
    }
}


