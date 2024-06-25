#include "avcore.h"

#include <iostream>

int AVCore::get_format_info(int fmt_ctx_idx,
                            nlohmann::json &info)
{
    AVFormatContext *fmt_ctx = nullptr;
    this->fmt_ctx_mgr->get_format_context(fmt_ctx_idx,
                                          fmt_ctx);
    dump_format_info(fmt_ctx_idx,
                     fmt_ctx,
                     info);
    return 0;
}

int AVCore::get_stream_info(int strm_idx,
                            nlohmann::json &info)
{
    int fmt_ctx_idx = -1;
    int in_fmt_strm_idx = -1;
    this->info_mgr->stream_to_format(strm_idx,
                                     fmt_ctx_idx,
                                     in_fmt_strm_idx);
    AVStream *strm = nullptr;
    this->fmt_ctx_mgr->get_stream(fmt_ctx_idx,
                                  in_fmt_strm_idx,
                                  strm);
    dump_stream_info(strm_idx,
                     strm,
                     info);
    return 0;
}

int AVCore::get_output_formats(nlohmann::json &formats)
{
    formats.clear();

    // get output formats
    std::vector<OSFFFormatInfo *> ofmts;
    this->info_mgr->get_output_formats(ofmts);

    for (auto it = ofmts.begin();
         it != ofmts.end();
         it++)
    {
        std::string fmt_idx = std::to_string((*it)->fmt_idx);
        formats[fmt_idx]["format_index"] = (*it)->fmt_idx;
        formats[fmt_idx]["url"] = (*it)->url;
        formats[fmt_idx]["io_type"] = (*it)->io_type;
    }

    return 0;
}

int AVCore::get_decode_streams(nlohmann::json &strms)
{
    strms.clear();

    // get decode streams
    std::vector<OSFFStreamInfo *> dstrms;
    this->info_mgr->get_decode_streams(dstrms);

    for (auto it = dstrms.begin();
         it != dstrms.end();
         it++)
    {
        int strm_idx = (*it)->strm_idx;
        strms[strm_idx]["stream_index"] = (*it)->strm_idx;
        strms[strm_idx]["format_index"] = (*it)->fmt_idx;
        strms[strm_idx]["in_format_stream_index"] = (*it)->in_fmt_strm_idx;
        strms[strm_idx]["media_type"] = (*it)->media_type;
    }

    return 0;
}
