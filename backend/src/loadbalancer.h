/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef LOADBALANCER_H_
#define LOADBALANCER_H_

/*
 * Instances of LoadBalancer class deal out jobs represented by integers from
 * the range 0..N-1 putting more emphasis on long-standing jobs.
 *
 * Users of this class should first call get() to get a job id. On completion
 * always call finish() with the same job id.
 */

#include <vector>
#include <thread>
#include <mutex>

namespace dmcr
{

class LoadBalancer
{
    public:
    LoadBalancer(size_t N);

    size_t get();
    void finish(size_t job);

    private:
    std::vector<int> m_pending;
    std::vector<int> m_finished;
    std::mutex       m_mutex;
};

}

#endif


