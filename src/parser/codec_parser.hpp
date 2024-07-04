#ifndef SNAKE_EYE_CODEC_PARSER_HPP
#define SNAKE_EYE_CODEC_PARSER_HPP

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/samplefmt.h>
#include <libavutil/channel_layout.h>
}
#include <nlohmann/json.hpp>

#include "../util/util.hpp"

namespace SnakeEye
{
    class SnakeEyeCodecParser
    {
    private:
        /**
         * @brief Get the list of profiles of a specific codec
         * @param[in] codec the codec
         * @param[out] profiles a json object with the following fields:
         *          - name: parameter name
         *          - help: help information
         *          - type: type of the option
         *          - values: the list of profiles, each profile object with the following fields:
         *              -# name: name of the profile
         *              -# value: int value of the profile
         */
        int get_encoder_profiles(const AVCodec *codec,
                                 nlohmann::json &profiles);

        int get_encoder_sample_formats(const AVCodec *codec,
                                       nlohmann::json &smp_fmts);

        int get_encoder_sample_rates(const AVCodec *codec,
                                     nlohmann::json &smp_rates);

        int get_encoder_channel_layouts(const AVCodec *codec,
                                        nlohmann::json &ch_layouts);

        int get_encoder_pixel_formats(const AVCodec *codec,
                                      nlohmann::json &pix_fmts);

        int get_encoder_bit_rate(nlohmann::json &bit_rate);

        int get_encoder_parameters(const AVCodec *codec,
                                   nlohmann::json &params);

        int parse_encoder_settings(nlohmann::json &settings,
                                   int &profile,
                                   int &bit_rate,
                                   AVSampleFormat &smp_fmt,
                                   int &smp_rate,
                                   AVChannelLayout &ch_layout,
                                   AVPixelFormat &pix_fmt,
                                   AVDictionary **priv_options);

    public:
        int list_decoders(nlohmann::json &decoders);

        /**
         * @brief Get the list of encoders of a specific codec type
         * @param[in] codec_type codec type, "audio" or "video"
         * @param[out] encoders a json object to store the list of encoders, each encoder object with the following fields:
         *            - name: name of the encoder
         *            - long_name: long name of the encoder
         * @return 0 on success, AVERROR code otherwise
         */
        int list_encoders(std::string codec_type,
                          nlohmann::json &encoders);

        int get_encoder_settings(std::string codec_name,
                                 nlohmann::json &params);

        int build_encoder_context(AVStream *dstrm,
                                  nlohmann::json &params_info,
                                  AVCodecContext *&codec_ctx);
    };
}

#endif // SNAKE_EYE_CODEC_PARSER_HPP
