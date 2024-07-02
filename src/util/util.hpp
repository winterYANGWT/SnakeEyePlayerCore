#ifndef SNAKE_EYE_UTIL_HPP
#define SNAKE_EYE_UTIL_HPP

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavutil/frame.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libavutil/error.h>
#include <libavutil/opt.h>
#include <libavutil/dict.h>
}
#include <string>
#include <nlohmann/json.hpp>

namespace SnakeEye
{
    // time_op.cpp
    int64_t time_sub(int64_t time1,
                     AVRational time_base1,
                     int64_t time2,
                     AVRational time_base2);

    // err.cpp
    std::string err2str(int errnum);

    // data_alloc.cpp
    int init_frame(AVFrame **frm);

    int init_frame_with_codec_ctx(AVFrame **frm,
                                  const AVCodecContext *codec_ctx);

    int init_audio_frame(AVFrame *&frm,
                         int nb_samples,
                         AVSampleFormat sample_fmt,
                         AVChannelLayout channel_layout,
                         int sample_rate);

    int init_video_frame(AVFrame *&frm,
                         int width,
                         int height,
                         AVPixelFormat pix_fmt);

    int init_packet(AVPacket **pkt);

    // dump_info.cpp
    int dump_format_info(int fmt_ctx_idx,
                         AVFormatContext *fmt_ctx,
                         nlohmann::json &info);

    int dump_stream_info(int strm_idx,
                         AVStream *strm,
                         nlohmann::json &info);

    // list.cpp
    int list_demuxers(nlohmann::json &demuxers);

    int list_muxers(nlohmann::json &muxers);

    int list_filters(nlohmann::json &filters);

    // get_param.cpp
    int parse_options(const AVClass **av_class,
                      nlohmann::json &params);

    int get_filter_parameters(std::string fltr_name,
                              nlohmann::json &param);

    int get_decoder_parameters(std::string codec_name,
                               nlohmann::json &param);

    int get_encoder_parameters(std::string codec_name,
                               nlohmann::json &param);

    int get_input_parameters(std::string fmt_name,
                             nlohmann::json &param);

    int get_output_parameters(std::string fmt_name,
                              nlohmann::json &param);

    int get_encoder_profiles(std::string codec_name,
                             nlohmann::json &supported_profiles);

    int get_encoder_sample_rates(std::string codec_name,
                                 nlohmann::json &supported_samplerates);

    int get_encoder_sample_formats(std::string codec_name,
                                   nlohmann::json &supported_sample_fmts);

    int get_encoder_pixel_formats(std::string codec_name,
                                  nlohmann::json &supported_pix_fmts);

    int get_encoder_channel_layouts(std::string codec_name,
                                    nlohmann::json &supported_channel_layouts);

    int parse_encoder_params(nlohmann::json &params,
                             AVDictionary **priv_options,
                             AVSampleFormat &smp_fmt,
                             int &smp_rate,
                             AVChannelLayout &ch_layout,
                             AVPixelFormat &pix_fmt);
}

#endif // SNAKE_EYE_UTIL_HPP
