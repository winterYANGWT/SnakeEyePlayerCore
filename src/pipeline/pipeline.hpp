#ifndef SNAKE_EYE_PIPELINE_HPP
#define SNAKE_EYE_PIPELINE_HPP

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <vector>
#include <string>

namespace SnakeEye
{
    class SnakeEyePipeline
    {
    public:
        std::vector<std::string> status_link_list;

        int ifmt_idx = -1;

        int ofmt_idx = -1;

        int istrm_idx = -1;

        int ostrm_idx = -1;

        int dstrm_idx = -1;

        int estrm_idx = -1;

        AVStream *dstrm = nullptr;

        AVStream *estrm = nullptr;

        AVCodecContext *dec_ctx = nullptr;

        AVCodecContext *enc_ctx = nullptr;
    };
}

#endif // SNAKE_EYE_PIPELINE_HPP
