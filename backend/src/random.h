/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_RANDOM_H
#define DMCR_RANDOM_H

#include "vector.h"

// Utilities for generating random numbers
namespace dmcr
{

class RNG
{
    public:
    /*
     * Return random double between [0, 1)
     */
    double random();

    /*
     * Return random unit vector
     */
    Vector3f random_vector();

    RNG();
    ~RNG();

    private:

    struct impl;
    impl* m;
};
}

#endif // DMCR_RANDOM_H
