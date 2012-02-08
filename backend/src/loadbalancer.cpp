#include <cassert>
#include "loadbalancer.h"


namespace dmcr {


LoadBalancer::LoadBalancer(size_t N) 
     : m_pending(N, 0)
     , m_finished(N, 0)
{
    // m_pending or m_finished should never change size
    assert(N > 0);
}

size_t LoadBalancer::get()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // The job that we should deal out is the one with least completions and 
    // least tasks working on it, ie. find the minimum sum 

    size_t min_index = 0;
    int min_sum = -1;
    
    for(size_t i = 0; i < m_pending.size(); ++i)
    {
        int sum = m_pending[i] + m_finished[i];
        if (sum < min_sum || min_sum == -1)
        {
            sum = min_sum;
            min_index = i;
        }
    }

    ++ m_pending[min_index];
    return min_index;
}

void LoadBalancer::finish(size_t job)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(job >= m_pending.size()) return; // huh.

    -- m_pending[job];
    ++ m_finished[job];

    // Make sure the counters in m_finished don't grow too much by scaling
    // them down whenever possible. 

    // See if all are > 0
    for(const int& val : m_finished)
    {
        if (val== 0)
            return; // Get out
    }
    // Remove one from each
    for(int& val : m_finished)
    {
        -- val;  
    }
}



}

