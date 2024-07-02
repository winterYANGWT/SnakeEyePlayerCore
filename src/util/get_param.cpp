#include "util.hpp"

static void process_default_string(nlohmann::json &param,
                                   const AVOption *option)
{
    if (option->default_val.str)
    {
        param["default"] = option->default_val.str;
    }
    else
    {
        param["default"] = "";
    }
}

static void process_help_string(nlohmann::json &param,
                                const AVOption *option)
{
    if (option->help)
    {
        param["help"] = option->help;
    }
    else
    {
        param["help"] = "";
    }
}

int SnakeEye::parse_options(const AVClass **av_class,
                            nlohmann::json &params)
{
    const AVOption *option = nullptr;
    const AVOption *last_option = nullptr;

    while ((option = av_opt_next(av_class, option)) != nullptr)
    {
        nlohmann::json param;
        std::string option_name = option->name;
        param["name"] = option_name;
        process_help_string(param, option);
        param["min"] = option->min;
        param["max"] = option->max;

        if (option->type == AV_OPT_TYPE_INT)
        {
            param["type"] = "int";
            param["default"] = option->default_val.i64;

            if (option->max != 0)
            {
                last_option = option;
                option = av_opt_next(av_class, option);

                if (option != nullptr &&
                    option->type == AV_OPT_TYPE_CONST)
                {
                    param["type"] = "int_enum";
                    std::vector<nlohmann::json> all_values;

                    do
                    {
                        nlohmann::json value;
                        value["name"] = option->name;
                        value["value"] = option->default_val.i64;
                        all_values.push_back(value);
                        last_option = option;
                        option = av_opt_next(av_class, option);
                    } while (option != nullptr &&
                             option->type == AV_OPT_TYPE_CONST);

                    param["values"] = all_values;
                }

                option = last_option;
            }
        }
        else if (option->type == AV_OPT_TYPE_INT64 ||
                 option->type == AV_OPT_TYPE_UINT64)
        {
            param["type"] = "int64";
            param["default"] = option->default_val.i64;
        }
        else if (option->type == AV_OPT_TYPE_FLOAT ||
                 option->type == AV_OPT_TYPE_DOUBLE)
        {
            param["type"] = "double";
            param["default"] = option->default_val.dbl;
        }
        else if (option->type == AV_OPT_TYPE_STRING)
        {
            param["type"] = "string";
            process_default_string(param, option);
        }
        else if (option->type == AV_OPT_TYPE_RATIONAL)
        {
            param["type"] = "rational";
            param["default"] = std::to_string(option->default_val.q.num) +
                               "/" +
                               std::to_string(option->default_val.q.den);
        }
        else if (option->type == AV_OPT_TYPE_BOOL)
        {
            param["type"] = "bool";
            param["default"] = option->default_val.i64 == 0 ? false : true;
        }
        else if (option->type == AV_OPT_TYPE_IMAGE_SIZE)
        {
            param["type"] = "image_size";
        }
        else if (option->type == AV_OPT_TYPE_VIDEO_RATE)
        {
            param["type"] = "video_rate";
            process_default_string(params, option);
        }
        else if (option->type == AV_OPT_TYPE_COLOR)
        {
            param["type"] = "color";
            process_default_string(params, option);
        }
        else
        {
            param["type"] = "unknown";
            param["default"] = "unknown";
        }

        params.push_back(param);
    }

    return 0;
}

int SnakeEye::get_filter_parameters(std::string fltr_name,
                                    nlohmann::json &params)
{
    const AVFilter *fltr = avfilter_get_by_name(fltr_name.c_str());

    if (fltr == nullptr)
    {
        return AVERROR_FILTER_NOT_FOUND;
    }

    const AVClass *avclass = fltr->priv_class;
    parse_options(&avclass, params);
    return 0;
}

int SnakeEye::get_decoder_parameters(std::string codec_name,
                                     nlohmann::json &params)
{
    const AVCodec *codec = avcodec_find_decoder_by_name(codec_name.c_str());

    if (codec == nullptr)
    {
        return AVERROR_DECODER_NOT_FOUND;
    }

    const AVClass *avclass = codec->priv_class;
    parse_options(&avclass, params);
    return 0;
}

int SnakeEye::get_encoder_parameters(std::string codec_name,
                                     nlohmann::json &params)
{
    const AVCodec *codec = avcodec_find_encoder_by_name(codec_name.c_str());

    if (codec == nullptr)
    {
        return AVERROR_ENCODER_NOT_FOUND;
    }

    const AVClass *avclass = codec->priv_class;
    parse_options(&avclass, params);
    return 0;
}

int SnakeEye::get_input_parameters(std::string fmt_name,
                                   nlohmann::json &params)
{
    const AVInputFormat *fmt = av_find_input_format(fmt_name.c_str());

    if (fmt == nullptr)
    {
        return AVERROR_DEMUXER_NOT_FOUND;
    }

    const AVClass *avclass = fmt->priv_class;
    parse_options(&avclass, params);
    return 0;
}

int SnakeEye::get_output_parameters(std::string fmt_name,
                                    nlohmann::json &params)
{
    const AVOutputFormat *fmt = av_guess_format(fmt_name.c_str(),
                                                nullptr,
                                                nullptr);

    if (fmt == nullptr)
    {
        return AVERROR_MUXER_NOT_FOUND;
    }

    const AVClass *avclass = fmt->priv_class;
    parse_options(&avclass, params);
    return 0;
}
