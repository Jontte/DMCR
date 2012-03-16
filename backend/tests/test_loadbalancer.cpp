/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <cassert>
#include <iostream>
#include <vector>
#include "../src/loadbalancer.h"


void test_loadbalancer()
{
    dmcr::LoadBalancer b(10, 20);

    std::vector<int> vals(10, 0);
    
    // Get 100 jobs, resulting in even load of 10 per job-id
    for(int i = 0 ; i < 100; ++i)
    {
        int ret = b.get();
        assert(ret != -1);
        ++vals.at(ret);
    }

    // Check each is 10
    for(int i = 0; i < 10; ++i)
    {
      assert(vals[i] == 10);
    }

    // Finish all pending 0-jobs
    for(int i = 0; i < 10; ++i)
        b.finish(0);

    // The next 10 calls should not give 0s since they're all finished and 
    // the rest seem a lot heavier computationally
    for(int i = 0; i < 10; ++i)
    {
        int ret = b.get();
        assert(ret != -1 && ret != 0);
    }
    // The system should return jobs 90 times until running out of work
    for(int i = 0; i < 90; i++)
    {
        int ret = b.get();
        assert(ret != -1);
    }
    // All done
    int ret = b.get();
    assert(ret == -1);
}


