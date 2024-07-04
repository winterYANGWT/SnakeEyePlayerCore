#include "codec_parser.hpp"

#include <iostream>

int SnakeEye::SnakeEyeCodecParser::get_encoder_profiles(const AVCodec *codec,
                                                        nlohmann::json &profiles)
{
    profiles.clear();
    profiles["name"] = "encoder_profiles";
    profiles["help"] = "The profiles supported by the encoder";
    profiles["type"] = "int_enum";
    profiles["values"] = nlohmann::json::array();

    for (const AVProfile *profile = codec->profiles;
         profile != nullptr &&
         profile->profile != AV_PROFILE_UNKNOWN;
         profile++)
    {
        nlohmann::json profile_info;
        profile_info["name"] = profile->name;
        profile_info["value"] = (int)profile->profile;
        profiles["values"].push_back(profile_info);
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_sample_formats(const AVCodec *codec,
                                                              nlohmann::json &smp_fmts)
{
    smp_fmts.clear();
    smp_fmts["name"] = "encoder_sample_formats";
    smp_fmts["help"] = "The sample formats supported by the encoder";
    smp_fmts["type"] = "int_enum";
    smp_fmts["values"] = nlohmann::json::array();

    for (const AVSampleFormat *smp_fmt = codec->sample_fmts;
         smp_fmt != nullptr &&
         *smp_fmt != AV_SAMPLE_FMT_NONE;
         smp_fmt++)
    {
        nlohmann::json smp_fmt_info;
        smp_fmt_info["name"] = av_get_sample_fmt_name(*smp_fmt);
        smp_fmt_info["value"] = (int)*smp_fmt;
        smp_fmts["values"].push_back(smp_fmt_info);
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_sample_rates(const AVCodec *codec, nlohmann::json &smp_rates)
{
    smp_rates.clear();
    smp_rates["name"] = "encoder_sample_rates";
    smp_rates["help"] = "The sample rates supported by the encoder";
    smp_rates["type"] = "int_enum";
    smp_rates["values"] = nlohmann::json::array();

    for (const int *smp_rate = codec->supported_samplerates;
         smp_rate != nullptr &&
         *smp_rate != 0;
         smp_rate++)
    {
        nlohmann::json smp_rate_info;
        smp_rate_info["name"] = std::to_string(*smp_rate);
        smp_rate_info["value"] = *smp_rate;
        smp_rates["values"].push_back(smp_rate_info);
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_channel_layouts(const AVCodec *codec,
                                                               nlohmann::json &ch_layouts)
{
    ch_layouts.clear();
    ch_layouts["name"] = "encoder_channel_layouts";
    ch_layouts["help"] = "The channel layouts supported by the encoder";
    ch_layouts["type"] = "string_enum";
    ch_layouts["values"] = nlohmann::json::array();
    char buf[128] = {0};

    for (const AVChannelLayout *ch_layout = codec->ch_layouts;
         ch_layout != nullptr &&
         ch_layout->nb_channels != 0;
         ch_layout++)
    {
        nlohmann::json ch_layout_info;
        av_channel_layout_describe(ch_layout, buf, 128);
        ch_layout_info["name"] = buf;
        ch_layout_info["value"] = buf;
        ch_layouts["values"].push_back(ch_layout_info);
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_pixel_formats(const AVCodec *codec,
                                                             nlohmann::json &pix_fmts)
{
    pix_fmts.clear();
    pix_fmts["name"] = "encoder_pixel_formats";
    pix_fmts["help"] = "The pixel formats supported by the encoder";
    pix_fmts["type"] = "int_enum";
    pix_fmts["values"] = nlohmann::json::array();

    for (const AVPixelFormat *pix_fmt = codec->pix_fmts;
         pix_fmt != nullptr &&
         *pix_fmt != AV_PIX_FMT_NONE;
         pix_fmt++)
    {
        nlohmann::json pix_fmt_info;
        pix_fmt_info["name"] = av_get_pix_fmt_name(*pix_fmt);
        pix_fmt_info["value"] = (int)*pix_fmt;
        pix_fmts["values"].push_back(pix_fmt_info);
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_bit_rate(nlohmann::json &bit_rate)
{
    bit_rate.clear();
    bit_rate["name"] = "bit_rate";
    bit_rate["help"] = "The bit rate of the encoder";
    bit_rate["type"] = "int";
    bit_rate["default"] = 0;
    bit_rate["min"] = 0;
    bit_rate["max"] = INT_MAX;
    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_parameters(const AVCodec *codec,
                                                          nlohmann::json &params)
{
    params.clear();
    params["name"] = "encoder_params";
    params["help"] = "The parameters of the encoder";
    params["type"] = "multi_fields";
    params["values"] = nlohmann::json::array();
    const AVClass *av_class = codec->priv_class;
    parse_options(&av_class, params["values"]);
    return 0;
}

int SnakeEye::SnakeEyeCodecParser::parse_encoder_settings(nlohmann::json &settings,
                                                          int &profile,
                                                          int &bit_rate,
                                                          AVSampleFormat &smp_fmt,
                                                          int &smp_rate,
                                                          AVChannelLayout &ch_layout,
                                                          AVPixelFormat &pix_fmt,
                                                          AVDictionary **priv_options)
{
    for (auto it = settings.begin();
         it != settings.end();
         it++)
    {
        if ((*it)["type"] == "multi_fields")
        {
            for (auto it2 = (*it)["values"].begin();
                 it2 != (*it)["values"].end();
                 ++it2)
            {
                if (it2->find("value") != it2->end())
                {
                    std::string type = it2->at("type").get<std::string>();
                    std::string name = it2->at("name").get<std::string>();
                    std::string value;

                    if (type == "int")
                    {
                        value = std::to_string(it2->at("value").get<int>());
                    }
                    else if (type == "double")
                    {
                        value = std::to_string(it2->at("value").get<double>());
                    }
                    else if (type == "bool")
                    {
                        value = std::to_string(it2->at("value").get<bool>());
                    }
                    else if (type == "string")
                    {
                        value = it2->at("value").get<std::string>();
                    }
                    else
                    {
                        continue;
                    }

                    av_dict_set(priv_options,
                                name.c_str(),
                                value.c_str(),
                                0);
                }
            }
        }
        else if ((*it)["name"] == "encoder_profiles" &&
                 it->find("value") != it->end())
        {
            profile = it->at("value").get<int>();
        }
        else if ((*it)["name"] == "bit_rate" &&
                 it->find("value") != it->end())
        {
            bit_rate = it->at("value").get<int>();
        }
        else if ((*it)["name"] == "encoder_sample_formats" &&
                 it->find("value") != it->end())
        {
            smp_fmt = (AVSampleFormat)it->at("value").get<int>();
        }
        else if ((*it)["name"] == "encoder_sample_rates" &&
                 it->find("value") != it->end())
        {
            smp_rate = it->at("value").get<int>();
        }
        else if ((*it)["name"] == "encoder_channel_layouts" &&
                 it->find("value") != it->end())
        {
            av_channel_layout_from_string(&ch_layout,
                                          it->at("value").get<std::string>().c_str());
        }
        else if ((*it)["name"] == "encoder_pixel_formats" &&
                 it->find("value") != it->end())
        {
            pix_fmt = (AVPixelFormat)it->at("value").get<int>();
        }
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::list_decoders(nlohmann::json &decoders)
{
    void *opaque = nullptr;
    const AVCodec *codec = nullptr;

    while ((codec = av_codec_iterate(&opaque)) != nullptr)
    {
        if (av_codec_is_decoder(codec) &&
            (codec->type == AVMEDIA_TYPE_VIDEO ||
             codec->type == AVMEDIA_TYPE_AUDIO))
        {
            decoders[codec->name]["name"] = codec->name;
            decoders[codec->name]["long_name"] = codec->long_name;
        }
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::list_encoders(std::string codec_type,
                                                 nlohmann::json &encoders)
{
    encoders.clear();
    encoders = nlohmann::json::array();
    void *opaque = nullptr;
    const AVCodec *codec = nullptr;

    while ((codec = av_codec_iterate(&opaque)) != nullptr)
    {
        if (av_codec_is_encoder(codec))
        {
            if ((codec_type == "audio" &&
                 codec->type == AVMEDIA_TYPE_AUDIO) ||

                (codec_type == "video" &&
                 codec->type == AVMEDIA_TYPE_VIDEO))
            {
                nlohmann::json encoder_info;
                encoder_info["name"] = codec->name;
                encoder_info["long_name"] = codec->long_name;
                encoders.push_back(encoder_info);
            }
        }
    }

    return 0;
}

int SnakeEye::SnakeEyeCodecParser::get_encoder_settings(std::string codec_name,
                                                        nlohmann::json &settings)
{
    const AVCodec *codec = avcodec_find_encoder_by_name(codec_name.c_str());

    if (codec == nullptr)
    {
        return AVERROR_ENCODER_NOT_FOUND;
    }

    settings.clear();
    settings = nlohmann::json::array();

    // get profile options
    nlohmann::json profiles;
    this->get_encoder_profiles(codec, profiles);
    settings.push_back(profiles);

    // get bit rate options
    nlohmann::json bit_rate;
    this->get_encoder_bit_rate(bit_rate);
    settings.push_back(bit_rate);

    if (codec->type == AVMEDIA_TYPE_AUDIO)
    {
        // get sample formats
        nlohmann::json smp_fmts;
        this->get_encoder_sample_formats(codec, smp_fmts);
        settings.push_back(smp_fmts);

        // get sample rates
        nlohmann::json smp_rates;
        this->get_encoder_sample_rates(codec, smp_rates);
        settings.push_back(smp_rates);

        // get channel layouts
        nlohmann::json ch_layouts;
        this->get_encoder_channel_layouts(codec, ch_layouts);
        settings.push_back(ch_layouts);
    }
    else if (codec->type == AVMEDIA_TYPE_VIDEO)
    {
        // get pixel formats
        nlohmann::json pix_fmts;
        this->get_encoder_pixel_formats(codec, pix_fmts);
        settings.push_back(pix_fmts);
    }

    // get encoder parameters
    nlohmann::json encoder_params;
    this->get_encoder_parameters(codec, encoder_params);
    settings.push_back(encoder_params);
    return 0;
}

int SnakeEye::SnakeEyeCodecParser::build_encoder_context(AVStream *dstrm,
                                                         nlohmann::json &params_info,
                                                         AVCodecContext *&codec_ctx)
{
    // get codec
    std::cout << params_info << std::endl;
    std::string codec_name = params_info["encoder_name"].get<std::string>();
    const AVCodec *codec = avcodec_find_encoder_by_name(codec_name.c_str());

    if (codec == nullptr)
    {
        return AVERROR_ENCODER_NOT_FOUND;
    }

    // create codec context
    if ((codec_ctx = avcodec_alloc_context3(codec)) == nullptr)
    {
        return AVERROR(ENOMEM);
    }

    // parse codec parameters
    AVSampleFormat smp_fmt = AV_SAMPLE_FMT_NONE;
    int profile = AV_PROFILE_UNKNOWN;
    int bit_rate = -1;
    int smp_rate = -1;
    AVChannelLayout ch_layout;
    ch_layout.nb_channels = 0;
    AVPixelFormat pix_fmt = AV_PIX_FMT_NONE;
    AVDictionary *priv_options = nullptr;

    if (this->parse_encoder_settings(params_info["encoder_settings"],
                                     profile,
                                     bit_rate,
                                     smp_fmt,
                                     smp_rate,
                                     ch_layout,
                                     pix_fmt,
                                     &priv_options) < 0)
    {
        return AVERROR(EINVAL);
    }

    if (profile != AV_PROFILE_UNKNOWN)
    {
        codec_ctx->profile = profile;
    }

    if (bit_rate != -1)
    {
        codec_ctx->bit_rate = bit_rate;
    }

    if (dstrm->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        if (smp_fmt != AV_SAMPLE_FMT_NONE)
        {
            codec_ctx->sample_fmt = smp_fmt;
        }
        else
        {
            codec_ctx->sample_fmt = (AVSampleFormat)dstrm->codecpar->format;
        }

        if (smp_rate != -1)
        {
            codec_ctx->sample_rate = smp_rate;
        }
        else
        {
            codec_ctx->sample_rate = dstrm->codecpar->sample_rate;
        }

        if (ch_layout.nb_channels != 0)
        {
            av_channel_layout_copy(&codec_ctx->ch_layout,
                                   &ch_layout);
        }
        else
        {
            av_channel_layout_default(&codec_ctx->ch_layout,
                                      dstrm->codecpar->ch_layout.nb_channels);
        }

        codec_ctx->time_base.num = 1;
        codec_ctx->time_base.den = codec_ctx->sample_rate;
    }
    else if (dstrm->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        if (pix_fmt != AV_PIX_FMT_NONE)
        {
            codec_ctx->pix_fmt = pix_fmt;
        }
        else
        {
            codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
        }

        codec_ctx->width = dstrm->codecpar->width;
        codec_ctx->height = dstrm->codecpar->height;
        codec_ctx->time_base = dstrm->time_base;
        codec_ctx->framerate = dstrm->avg_frame_rate;
    }
    else
    {
        return AVERROR(EINVAL);
    }

    if ((avcodec_open2(codec_ctx,
                       codec,
                       &priv_options)) < 0)
    {
        return AVERROR(EINVAL);
    }

    return 0;
}
