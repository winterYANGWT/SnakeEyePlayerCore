#ifndef SNAKE_EYE_CLOCK_HPP
#define SNAKE_EYE_CLOCK_HPP

extern "C"
{
#include <libavutil/time.h>
}

#include "../static/marco.hpp"

namespace SnakeEye
{
    class SnakeEyeClock
    {
    private:
        double pts = 0.0;

        double pts_drift = 0.0;

        double last_updated = 0.0;

        double speed = 1.0;

    public:
        SnakeEyeClock()
        {
            this->speed = 1.0;
            this->set_clock(NAN);
        }

        void set_clock_at(double pts,
                          double time)
        {
            this->pts = pts;
            this->last_updated = time;
            this->pts_drift = this->pts - time;
        }

        void set_clock(double pts)
        {
            double time = av_gettime_relative() / 1000000.0;
            this->set_clock_at(pts, time);
        }

        double get_clock()
        {
            double time = av_gettime_relative() / 1000000.0;
            return this->pts_drift + time - (time - this->last_updated) * (1.0 - this->speed);
        }
    };
}

#endif // SNAKE_EYE_CLOCK_HPP
