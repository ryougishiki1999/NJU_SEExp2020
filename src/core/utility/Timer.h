#ifndef UTILITY_TIMER_H
#define UTILITY_TIMER_H

#include <chrono>

class Timer
{
  public:
    /*! \brief record the 'start time'
     */
    void start() noexcept
    {
        start_t = std::chrono::steady_clock::now();
    }
    /*! \brief record the 'stop time'
     */
    void stop() noexcept
    {
        end_t = std::chrono::steady_clock::now();
    }
    /*! \brief get the duration('stop time' - 'start time')
     */
    double used_time()
    {
        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_t - start_t);
        return duration.count();
    }

  private:
    std::chrono::steady_clock::time_point start_t;
    std::chrono::steady_clock::time_point end_t;
    std::chrono::duration<double> duration;
};
#endif