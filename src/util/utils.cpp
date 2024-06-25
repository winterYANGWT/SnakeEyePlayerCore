#include "utils.h"

std::string generate_device_name(const std::string format_name,
                                 const std::string &audio_device_name,
                                 const std::string &video_device_name)
{
    std::string device_name = "";

    if (format_name == "dshow")
    {
        if (audio_device_name != "")
        {
            device_name += std::string("audio=") +
                           audio_device_name;
        }

        if (video_device_name != "")
        {
            if (audio_device_name != "")
            {
                device_name += std::string(":");
            }

            device_name += std::string("video=") +
                           video_device_name;
        }
    }
    else if (format_name == "avfoundation")
    {
        if (video_device_name != "")
        {
            device_name += video_device_name;
        }

        device_name += std::string(":");

        if (audio_device_name != "")
        {
            device_name += audio_device_name;
        }
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR, "Format name: '%s' is not supported now.\n",
               format_name.c_str());
    }

    return device_name;
}