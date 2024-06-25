#include "format_info.h"

int OSFFFormatInfo::parse(int fmt_idx,
                          AVFormatContext *fmt_ctx)
{
    this->fmt_idx = fmt_idx;
    this->url = fmt_ctx->url;
    this->num_streams = fmt_ctx->nb_streams;
    this->bit_rate = fmt_ctx->bit_rate;
    this->start_time = av_q2d(AV_TIME_BASE_Q) * fmt_ctx->start_time;
    this->duration = av_q2d(AV_TIME_BASE_Q) * fmt_ctx->duration;

    if (fmt_ctx->iformat)
    {
        this->io_type = "input";
        this->fmt_name = fmt_ctx->iformat->name;
    }
    else if (fmt_ctx->oformat)
    {
        this->io_type = "output";
        this->fmt_name = fmt_ctx->oformat->name;
    }
    else
    {
        this->io_type = "unknown";
        this->fmt_name = "unknown";
    }
}

int OSFFFormatInfo::parse(nlohmann::json &info)
{
    this->fmt_idx = info["format_index"];
    this->io_type = info["io_type"];
    this->url = info["url"];
    this->fmt_name = info["format_name"];
    this->bit_rate = info["bit_rate"];
    this->start_time = info["start_time"];
    this->duration = info["duration"];

    for (auto it = info["stream_index_list"].begin();
         it != info["stream_index_list"].end();
         it++)
    {
        this->strm_idxs.push_back(*it);
    }

    this->num_streams = this->strm_idxs.size();
}

int OSFFFormatInfo::to_json(nlohmann::json &info)
{
    info.clear();
    info["format_index"] = this->fmt_idx;
    info["io_type"] = this->io_type;
    info["url"] = this->url;
    info["format_name"] = this->fmt_name;
    info["num_streams"] = this->strm_idxs.size();
    info["stream_index_list"] = this->strm_idxs;
    info["bit_rate"] = this->bit_rate;
    info["start_time"] = this->start_time;
    info["duration"] = this->duration;
    return 0;
}
