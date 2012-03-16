/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <cassert>
#include <iostream>
#include "loadbalancer.h"

namespace dmcr {

LoadBalancer::LoadBalancer(size_t n_jobs, int jobs_per_slice)
     : m_pending(n_jobs, 0)
     , m_finished(n_jobs, 0)
     , m_job_target(jobs_per_slice)
{
    // m_pending or m_finished should never change size
    assert(n_jobs > 0 && jobs_per_slice > 0);
}

int LoadBalancer::get()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Find job with work left and with worst ratios

    int min_index = -1;
    int min_sum = 0;
    bool first = true;
    
    for(size_t i = 0; i < m_pending.size(); ++i)
    {
        int sum = m_finished[i] + m_pending[i];
        if(sum >= m_job_target)
            continue;
        sum = m_finished[i]*100 / (sum+1) + sum; 
        if (sum < min_sum || first)
        {
            first = false;
            min_sum = sum;
            min_index = i;
        }
    }
    if( min_index < 0)
    {
        // Couldn't find a suitable job (All jobs exhausted)
        return -1;
    }

    ++ m_pending[min_index];
    return min_index;
}

void LoadBalancer::finish(int job)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(job < 0 || job >= (int)m_pending.size())
        return; // huh.

    -- m_pending[job];
    ++ m_finished[job];
}


}

