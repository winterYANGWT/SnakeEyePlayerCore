#ifndef SNAKE_EYE_STREAM_INFO_HPP
#define SNAKE_EYE_STREAM_INFO_HPP

extern "C"
{
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}
#include <string>

namespace SnakeEye
{
    class SnakeEyeStreamInfo
    {
    public:
        int strm_idx = -1;

        int fmt_idx = -1;

        int in_fmt_strm_idx = -1;

        std::string code_type;

        std::string media_type;

        SnakeEyeStreamInfo(int strm_idx,
                           int fmt_idx,
                           int in_fmt_strm_idx,
                           std::string code_type,
                           std::string media_type)
        {
            this->strm_idx = strm_idx;
            this->fmt_idx = fmt_idx;
            this->in_fmt_strm_idx = in_fmt_strm_idx;
            this->code_type = code_type;
            this->media_type = media_type;
        }
    };
}

#endif // SNAKE_EYE_STREAM_INFO_HPP
