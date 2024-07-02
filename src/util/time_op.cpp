#include "util.hpp"

int64_t SnakeEye::time_sub(int64_t time1, AVRational time_base1, int64_t time2, AVRational time_base2)
{
    int64_t base_time1 = av_rescale_q(time1, time_base1, (AVRational){1, AV_TIME_BASE});
    int64_t base_time2 = av_rescale_q(time2, time_base2, (AVRational){1, AV_TIME_BASE});
    return base_time1 - base_time2;
}
