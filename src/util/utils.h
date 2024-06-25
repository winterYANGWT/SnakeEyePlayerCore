#ifndef UTILS_H
#define UTILS_H

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

std::string generate_device_name(const std::string format_name,
                                 const std::string &audio_device_name,
                                 const std::string &video_device_name);

#endif // UTILS_H
