#ifndef SNAKE_EYE_UTILS_HPP
#define SNAKE_EYE_UTILS_HPP

extern "C"
{
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavutil/audio_fifo.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}
#include <string>

namespace SnakeEye
{
    std::string generate_device_name(const std::string format_name,
                                     const std::string &audio_device_name,
                                     const std::string &video_device_name);
}

#endif // SNAKE_EYE_UTILS_HPP
