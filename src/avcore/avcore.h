#ifndef AVCORE_H
#define AVCORE_H

extern "C"
{
#include <libavdevice/avdevice.h>
}
#include <string>
#include <nlohmann/json.hpp>

#include "../manager/format_context_manager.h"
#include "../manager/format_packet_transfer_manager.h"
#include "../manager/codec_context_manager.h"
#include "../manager/transformer_manager.h"
#include "../manager/play_manager.h"
#include "../manager/data_manager.h"
#include "../manager/info_manager.h"
#include "../util/util.h"
#include "../parser/codec_parser.h"

class AVCore
{
private:
    FormatContextManager *fmt_ctx_mgr = nullptr;

    FormatPacketTransferManager *fmt_pkt_trfm_mgr = nullptr;

    CodecContextManager *codec_ctx_mgr = nullptr;

    TransformerManager *trfm_mgr = nullptr;

    PlayManager *play_mgr = nullptr;

    DataManager *data_mgr = nullptr;

    InfoManager *info_mgr = nullptr;

    OSFFCodecParser *codec_parser = nullptr;

    void init();

    void clear();

private:
    int build_decode_encode_pipeline(int dstrm_idx,
                                     int ofmt_idx,
                                     nlohmann::json &enc_params_info,
                                     AVDictionary **fltr_options = nullptr);

    int build_copy_pipeline(int dstrm_idx,
                            int ofmt_idx);

public:
    AVCore()
    {
        avdevice_register_all();
        this->init();
    }

    ~AVCore()
    {
        this->clear();
    }

    int open_input(const std::string &url,
                   const AVInputFormat *fmt,
                   AVDictionary **options);

    int open_output(const std::string &url,
                    const AVOutputFormat *fmt,
                    AVDictionary **options);

    // play related functions
    int link_audio_play(int dec_strm_idx);

    int link_video_play(int dec_strm_idx);

    int peek_play_frame(int play_strm_idx,
                        PlayFrame *&frm);

    int peek_last_play_frame(int play_strm_idx,
                             PlayFrame *&frm);

    int peek_next_play_frame(int play_strm_idx,
                             PlayFrame *&frm);

    int play_frame_size(int play_strm_idx);

    int pop_play_stream_frame(int play_strm_idx);

    int set_clock_at(int play_strm_idx,
                     double pts,
                     double time);

    int set_clock(int play_strm_idx,
                  double pts);

    int get_clock(int play_strm_idx,
                  double &get_time);

    int set_frame_clock(int play_strm_idx,
                        double pts);

    int get_frame_clock(int play_strm_idx,
                        double &get_time);

    // general functions
    int set_stream_link(nlohmann::json &input,
                        nlohmann::json &output);

    int unset_stream_link(int enc_strm_idx);

    int start();

    int get_format_info(int fmt_ctx_idx,
                        nlohmann::json &info);

    int get_streams_index_by_format_index(int fmt_ctx_idx,
                                          nlohmann::json &info);

    int get_stream_info(int strm_idx,
                        nlohmann::json &info);

    /**
     * @brief Get the list of supported encoders
     * @param[in]  input a json object with the following fields:
     *             - codec_type: "audio" or "video"
     * @param[out] output a json object with the following fields:
     *             - codec_list: list of supported encoders, each object with the following fields:
     *                  -# name: name of the encoder.
     *                  -# long_name: long name of the encoder
     * @return 0 on success, AVERROR code otherwise
     */
    int get_encoder_list(nlohmann::json &input,
                         nlohmann::json &output)
    {
        std::string codec_type = input["codec_type"].get<std::string>();
        nlohmann::json encoders;
        this->codec_parser->list_encoders(codec_type,
                                          encoders);
        output["codec_list"] = encoders;
        return 0;
    }

    int get_output_formats(nlohmann::json &formats);

    int get_decode_streams(nlohmann::json &streams);

    int get_encoder_setting(nlohmann::json &input,
                            nlohmann::json &output)
    {
        std::string codec_name = input["codec_name"].get<std::string>();
        nlohmann::json encoder_settings;
        this->codec_parser->get_encoder_settings(codec_name,
                                                 encoder_settings);
        output["encoder_settings"] = encoder_settings;
        return 0;
    }

    // int open_output(const std::string &url);
};

#endif // AVCORE_H
