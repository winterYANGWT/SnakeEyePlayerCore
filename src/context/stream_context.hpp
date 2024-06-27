#ifndef SNAKE_EYE_STREAM_CONTEXT_HPP
#define SNAKE_EYE_STREAM_CONTEXT_HPP

extern "C"
{
#include <libavcodec/avcodec.h>
}

#include <string>

namespace SnakeEye
{
    class SnakeEyeStreamContext
    {
    public:
        int strm_id = -1;

        AVCodecContext *codec_ctx = nullptr;

        int strm_pkt_queue_id = -1;

        int strm_frm_queue_id = -1;

        std::string code_type = "";

        std::string codec_name = "";

        AVDictionary *opt = nullptr;

    public:
        SnakeEyeStreamContext(int strm_id,
                              AVCodecContext *strm_ctx,
                              const std::string &code_type,
                              const std::string &codec_name,
                              AVDictionary *opt)
            : strm_id(strm_id),
              codec_ctx(codec_ctx),
              code_type(code_type),
              codec_name(codec_name),
              opt(opt) {}

        ~SnakeEyeStreamContext()
        {
            if (this->codec_ctx)
            {
                avcodec_free_context(&this->codec_ctx);
            }

            if (this->opt)
            {
                av_dict_free(&this->opt);
            }
        }
    };
}

#endif // SNAKE_EYE_STREAM_CONTEXT_HPP
