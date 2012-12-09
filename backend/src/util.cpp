/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <vector>
#include "util.h"

#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

namespace dmcr
{
/*
class ProgressBar
{
    public:
        ProgressBar(unsigned int max_progress);
        void update(unsigned int progress);

        double speed() const;
        std::string render() const;
    private:
        double  m_avg_speed;
        int     m_last_progress;
        int     m_max_progress;
};*/

ProgressBar::ProgressBar(unsigned int max_progress)
#if GCC_VERSION < 407
    : m_start_time(std::chrono::monotonic_clock::now())
    , m_last_update_time(std::chrono::monotonic_clock::now())
#else
    : m_start_time(std::chrono::steady_clock::now())
    , m_last_update_time(std::chrono::steady_clock::now())
#endif
    , m_avg_speed(0)
    , m_last_progress(0)
    , m_max_progress(max_progress)
{}

double ProgressBar::speed() const
{
    return m_avg_speed;
}

void ProgressBar::update(unsigned int progress)
{
    if(progress > m_max_progress)
        progress = m_max_progress;
    if(progress < m_last_progress)
        progress = m_last_progress;

    using namespace std::chrono;
#if GCC_VERSION < 407
    typedef monotonic_clock clock;
#else
    typedef steady_clock clock;
#endif

    clock::time_point now = clock::now();

    duration<double> dur = now - m_start_time;

    // approximate speed:
    double approx_speed = progress / dur.count();

    double averager = 0.5;
    m_avg_speed = m_avg_speed*(1.0-averager) + approx_speed * averager;
    m_last_progress = progress;
    m_last_update_time = now;
}

std::string ProgressBar::render() const
{
    std::stringstream ss;
    double spd = speed();
    using namespace std::chrono;

#if GCC_VERSION < 407
    typedef monotonic_clock clock;
#else
    typedef steady_clock clock;
#endif


    clock::time_point now = clock::now();
    duration<double> dur = now - m_last_update_time;

    double progress = double(m_last_progress + spd * dur.count())/m_max_progress;

    int pbar_width = 40;
    int percentage = int(100*progress);
    int sym = int(pbar_width*progress);

    ss << "[";
    for(int i = 0; i < pbar_width; i++)
        ss << "=> "[ (i < sym)?0:(i==sym?1:2) ];
    ss << "] (";
    ss << percentage ;
    ss << "%) ETA ";

    if(spd == 0)
        ss << "INF";
    else
    {
        int64_t timeleft = int64_t((m_max_progress - m_last_progress) / spd) - dur.count();
        if(timeleft < 0) { timeleft = 0;}
        int64_t hours =   int64_t (timeleft / 3600.0);
        int64_t minutes = int64_t ((timeleft-hours*3600) / 60.0);
        int64_t seconds = int64_t ((timeleft-hours*3600-minutes*60));

        ss << hours << ":";
        ss.fill('0');
        ss.width(2);
        ss << minutes << ":";
        ss.fill('0');
        ss.width(2);
        ss << seconds;
    }
    return ss.str();
}

std::string SI_prefix(double in)
{
    size_t lev = 0;
    while(in >= 1000)
    {
        in /= 1000;
        lev++;
    }

    std::vector<std::string> prefixes = {"","k","M","G","T","P","E","Z","Y"};

    std::stringstream ret;
    ret.precision(3);
    ret << in << " " << ((lev>=prefixes.size())?"X":prefixes[lev]);
    return ret.str();
}

}


