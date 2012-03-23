/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "util.h"

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
    : m_last_update(std::chrono::system_clock::now())
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
    typedef high_resolution_clock clock;

    clock::time_point now = clock::now();

    // calculate time duration from last update
    duration<double> dur = now - m_last_update;

    // steps advanced?
    unsigned steps = progress - m_last_progress;

    // approximate speed:
    double approx_speed = steps / dur.count();

    double averager = 0.2;
    m_avg_speed = m_avg_speed*(1.0-averager) + approx_speed * averager;
    m_last_progress = progress;
    m_last_update = now;
}

std::string ProgressBar::render() const
{
    std::stringstream ss;
    double spd = speed();

    int pbar_width = 20;
    int percentage = int(double(100*m_last_progress)/m_max_progress);
    int sym = int(double(pbar_width*m_last_progress)/m_max_progress);

    ss << "[";
    for(int i = 0; i < 20; i++)
        ss << "=> "[ (i < sym)?0:(i==sym?1:2) ];
    ss << "] (";
    ss << percentage ;
    ss << "%) ETA ";

    if(spd == 0)
        ss << "INF";
    else
    {
        double timeleft = (m_max_progress - m_last_progress) / spd;

        int hours = int(timeleft / 3600.0);
        int minutes = int((timeleft-hours*60) / 60.0);
        int seconds = int((timeleft-hours*3600-minutes*60));

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


}


