/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <chrono>
#include <sstream>
#include <stdexcept>
#include <string>

namespace dmcr
{

struct invalid_lexical_cast : public std::runtime_error
{
    invalid_lexical_cast() : 
        std::runtime_error("invalid lexical cast") {}
};

template <class Dest, class Src>
Dest lexical_cast(const Src& src)
{
    std::stringstream ss;
    Dest dest;
    ss << src;
    ss >> dest;
    if(!ss)
        throw invalid_lexical_cast();
    return dest;
}

/*
 * A very simple class that makes progressbar drawing easy
 * It calculates the average progression speed and ETA
 */
class ProgressBar
{
    public:
        ProgressBar(unsigned int max_progress);
        void update(unsigned int progress);

        double speed() const;
        std::string render() const;

    private:
        std::chrono::system_clock::time_point
                      m_start_time;
        double        m_avg_speed;
        unsigned int  m_last_progress;
        unsigned int  m_max_progress;
};

/*
 * Pretty print a huge number by finding an approximate representation 
 * (accompanied with SI prefix)
 */

std::string SI_prefix(double in);

}


#endif


