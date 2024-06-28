#ifndef SNAKE_EYE_FORMAT_INFO_HPP
#define SNAKE_EYE_FORMAT_INFO_HPP

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include <string>
#include <vector>

namespace SnakeEye
{
    class SnakeEyeFormatInfo
    {
    public:
        int fmt_idx = -1;

        std::vector<int> strm_idxs;

        std::string io_type;

        std::string url;

        SnakeEyeFormatInfo(int fmt_idx,
                           std::string io_type,
                           std::string url)
        {
            this->fmt_idx = fmt_idx;
            this->io_type = io_type;
            this->url = url;
        }
    };
}

#endif // SNAKE_EYE_FORMAT_INFO_HPP
