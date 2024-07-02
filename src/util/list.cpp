#include "util.hpp"

int SnakeEye::list_demuxers(nlohmann::json &demuxers)
{
    void *opaque = nullptr;
    const AVInputFormat *fmt = nullptr;

    while ((fmt = av_demuxer_iterate(&opaque)) != nullptr)
    {
        demuxers[fmt->name]["name"] = fmt->name;
        demuxers[fmt->name]["long_name"] = fmt->long_name;
    }

    return 0;
}

int SnakeEye::list_muxers(nlohmann::json &muxers)
{
    void *opaque = nullptr;
    const AVOutputFormat *fmt = nullptr;

    while ((fmt = av_muxer_iterate(&opaque)) != nullptr)
    {
        muxers[fmt->name]["name"] = fmt->name;
        muxers[fmt->name]["long_name"] = fmt->long_name;
    }

    return 0;
}

int SnakeEye::list_filters(nlohmann::json &filters)
{
    void *opaque = nullptr;
    const AVFilter *filter = nullptr;

    while ((filter = av_filter_iterate(&opaque)) != nullptr)
    {
        filters[filter->name]["name"] = filter->name;
        filters[filter->name]["long_name"] = filter->description;
    }

    return 0;
}
