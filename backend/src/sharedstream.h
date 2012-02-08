#ifndef SHARED_STREAM_H_
#define SHARED_STREAM_H_

#include <list>
#include <condition_variable>

/*  
 *  SharedStream<T> is an abstract data queue that enables multiple producers 
 *  to write to a common stream handle.
*/

namespace dmcr {

template <class T>
class SharedStream
{
    public:
    
    void push(const T& value)
    {
        {
            // Limit method re-entry
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push_back(value);
        }
        // Wake up one waiting listener
        m_cond.notify_one();
    }
    T pull()
    {
        // Limit method re-entry
        std::unique_lock<std::mutex> lock(m_mutex);

        // Wait until stream has content (mutex is unlocked while waiting)
        m_cond.wait(lock, [this](){
            return !m_queue.empty();
        });
        // Mutex is now locked again
        // Pop content like a boss
        T ret = std::move(m_queue.front()); 
        m_queue.pop_front();
        return ret;
    }

    private:
    
    std::mutex              m_mutex;
    std::condition_variable m_cond;
    std::list<T>            m_queue;
};


};

#endif

