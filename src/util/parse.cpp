#include "util.hpp"

int SnakeEye::parse_encoder_params(nlohmann::json &params,
                                   AVDictionary **priv_options,
                                   AVSampleFormat &smp_fmt,
                                   int &smp_rate,
                                   AVChannelLayout &ch_layout,
                                   AVPixelFormat &pix_fmt)
{
    if (!params["encoder_supported_sample_formats"].empty())
    {
        smp_fmt = (AVSampleFormat)params["encoder_supported_sample_formats"].get<int>();
    }

    if (!params["encoder_supported_sample_rates"].empty())
    {
        smp_rate = params["encoder_supported_sample_rates"].get<int>();
    }

    if (!params["encoder_supported_channel_layouts"].empty())
    {
        av_channel_layout_from_string(&ch_layout, params["encoder_supported_channel_layouts"].get<std::string>().c_str());
    }

    if (!params["encoder_supported_pixel_formats"].empty())
    {
        pix_fmt = (AVPixelFormat)params["encoder_supported_pixel_formats"].get<int>();
    }

    for (auto it = params["encoder_params"].begin();
         it != params["encoder_params"].end();
         it++)
    {
        std::string type = it.value()["type"].get<std::string>();

        if (type == "int")
        {
            av_dict_set(priv_options,
                        it.key().c_str(),
                        std::to_string(it.value()["value"].get<int>()).c_str(),
                        0);
        }
        else if (type == "double")
        {
            av_dict_set(priv_options,
                        it.key().c_str(),
                        std::to_string(it.value()["value"].get<double>()).c_str(),
                        0);
        }
        else if (type == "bool")
        {
            av_dict_set(priv_options,
                        it.key().c_str(),
                        std::to_string(it.value()["value"].get<bool>()).c_str(),
                        0);
        }
        else if (type == "string")
        {
            av_dict_set(priv_options,
                        it.key().c_str(),
                        it.value()["value"].get<std::string>().c_str(),
                        0);
        }
    }
    return 0;
}
