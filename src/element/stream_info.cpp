#include "stream_info.hpp"

SnakeEye::SnakeEyeStreamInfo::SnakeEyeStreamInfo(int strm_idx,
                                                 int fmt_idx,
                                                 AVStream *strm,
                                                 std::string code_type)
{
    this->strm_idx = strm_idx;
    this->fmt_idx = fmt_idx;
    this->code_type = code_type;
    this->in_fmt_strm_idx = strm->index;
    this->time_base = strm->time_base;
    this->nb_frames = strm->nb_frames;
    const AVCodecDescriptor *desc = avcodec_descriptor_get(strm->codecpar->codec_id);
    this->codec_name = desc->long_name;
    this->bit_rate = strm->codecpar->bit_rate;

    if (strm->duration != AV_NOPTS_VALUE)
    {
        this->duration = av_q2d(strm->time_base) * strm->duration;
    }

    if (strm->start_time != AV_NOPTS_VALUE)
    {
        this->start_time = av_q2d(strm->time_base) * strm->start_time;
    }

    if (strm->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        this->media_type = "video";
        this->fmt = av_get_pix_fmt_name((AVPixelFormat)strm->codecpar->format);
        this->width = strm->codecpar->width;
        this->height = strm->codecpar->height;
        this->frame_rate = strm->avg_frame_rate;
    }
    else if (strm->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        this->media_type = "audio";
        this->fmt = av_get_sample_fmt_name((AVSampleFormat)strm->codecpar->format);
        this->num_channels = strm->codecpar->ch_layout.nb_channels;
        this->sample_rate = strm->codecpar->sample_rate;
    }
    else if (strm->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE)
    {
        this->media_type = "subtitle";
    }
    else
    {
        this->media_type = "unknown";
    }
}

SnakeEye::SnakeEyeStreamInfo::SnakeEyeStreamInfo(nlohmann::json &info)
{
    this->strm_idx = info["stream_index"];
    this->fmt_idx = info["format_index"];
    this->in_fmt_strm_idx = info["in_format_stream_index"];
    this->code_type = info["code_type"];
    this->media_type = info["media_type"];
    int time_base_num = 0;
    int time_base_den = 0;
    sscanf(info["time_base"].get<std::string>().c_str(), "%d/%d", &time_base_num, &time_base_den);
    this->time_base.num = time_base_num;
    this->time_base.den = time_base_den;
    this->nb_frames = info["num_frames"].get<int64_t>();
    this->codec_name = info["codec_name"].get<std::string>();
    this->bit_rate = info["bit_rate"].get<int64_t>();

    if (info["duration"].get<std::string>().c_str() != "N/A")
    {
        this->duration = std::stod(info["duration"].get<std::string>());
    }

    if (info["start_time"].get<std::string>().c_str() != "N/A")
    {
        this->start_time = std::stod(info["start_time"].get<std::string>());
    }

    if (this->media_type == "video")
    {
        this->fmt = info["pixel_format"].get<std::string>();
        sscanf(info["width:height"].get<std::string>().c_str(), "%d:%d", &this->width, &this->height);
        int frame_rate_num = 0;
        int frame_rate_den = 0;
        sscanf(info["frame_rate"].get<std::string>().c_str(), "%d/%d", &frame_rate_num, &frame_rate_den);
        this->frame_rate.num = frame_rate_num;
        this->frame_rate.den = frame_rate_den;
    }
    else if (this->code_type == "audio")
    {
        this->fmt = info["sample_format"].get<std::string>();
        this->num_channels = info["num_channels"].get<int>();
        this->sample_rate = info["sample_rate"].get<int>();
    }
}

int SnakeEye::SnakeEyeStreamInfo::to_json(nlohmann::json &info)
{
    info.clear();
    info["stream_index"] = this->strm_idx;
    info["format_index"] = this->fmt_idx;
    info["in_format_stream_index"] = this->in_fmt_strm_idx;
    info["code_type"] = this->code_type;
    info["time_base"] = std::to_string(this->time_base.num) + "/" + std::to_string(this->time_base.den);
    info["num_frames"] = this->nb_frames;
    info["codec_name"] = this->codec_name;
    info["bit_rate"] = this->bit_rate;
    info["media_type"] = this->media_type;

    if (this->duration > 0.0)
    {
        info["duration"] = std::to_string(this->duration);
    }
    else
    {
        info["duration"] = "N/A";
    }

    if (this->start_time >= 0.0)
    {
        info["start_time"] = std::to_string(this->start_time);
    }
    else
    {
        info["start_time"] = "N/A";
    }

    if (this->media_type == "video")
    {
        info["media_type"] = "video";
        info["pixel_format"] = this->fmt;
        info["width:height"] = std::to_string(this->width) + ":" + std::to_string(this->height);
        info["frame_rate"] = std::to_string(this->frame_rate.num) + "/" + std::to_string(this->frame_rate.den);
    }
    else if (this->code_type == "audio")
    {
        info["media_type"] = "audio";
        info["sample_format"] = this->fmt;
        info["num_channels"] = this->num_channels;
        info["sample_rate"] = this->sample_rate;
    }

    return 0;
}
