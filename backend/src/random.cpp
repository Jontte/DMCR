/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <random>
#include <thread>
#include <fstream>
#include "random.h"

// Utilities for generating random numbers
namespace dmcr
{

struct RNG::impl
{
    std::mt19937 engine;
};

RNG::RNG()
    : m(new impl)
{
    uint32_t seed = 0;
    std::ifstream fin("/dev/urandom");
    fin.read(reinterpret_cast<char*>(&seed), 4);
    fin.close();
    m->engine.seed(seed);
}
RNG::~RNG()
{
    delete m;   
}

double RNG::random()
{
    std::uniform_real_distribution<> real(0,1);
    return real(m->engine);
}

Vector3f RNG::random_vector()
{
    // Source: http://math.stackexchange.com/questions/44689/
    // how-to-find-a-random-axis-or-unit- vectorin-3d
    float theta = random()*2*M_PI;
    float z = -1 + 2*random();
    float t = sqrt(1-z*z);
    return Vector3f(t*cos(theta), t*sin(theta), z);
}

}

