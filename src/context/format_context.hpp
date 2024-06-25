#ifndef OSFF_FORMAT_CONTEXT_HPP
#define OSFF_FORMAT_CONTEXT_HPP

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <string>
#include <vector>

#include "../util/util.h"

class OSFFFormatContext
{
public:
    int fmt_id = -1;

    AVFormatContext *fmt_ctx = nullptr;

    std::vector<int> active_strms;

    int fmt_pkt_queue_id = -1;

    int num_strms = 0;

    std::string io_type = "";

    std::string url = "";

    std::string fmt_name = "";

    AVDictionary *opt = nullptr;

    OSFFFormatContext(int fmt_id,
                      AVFormatContext *fmt_ctx,
                      const std::string &io_type,
                      const std::string &url,
                      const std::string &fmt_name,
                      AVDictionary *opt)
        : fmt_id(fmt_id),
          fmt_ctx(fmt_ctx),
          io_type(io_type),
          url(url),
          fmt_name(fmt_name),
          opt(opt) {}

    ~OSFFFormatContext()
    {
        if (this->fmt_ctx)
        {
            avformat_close_input(&this->fmt_ctx);
        }

        if (this->opt)
        {
            av_dict_free(&this->opt);
        }
    }

    int add_stream(AVCodecContext *codec_ctx,
                   int &ostrm_id);

    int delete_stream(int ostrm_id);
};

#endif // OSFF_FORMAT_CONTEXT_HPP