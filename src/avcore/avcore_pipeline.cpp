#include "avcore.h"

int AVCore::build_decode_encode_pipeline(int dstrm_idx,
                                         int ofmt_idx,
                                         nlohmann::json &enc_info,
                                         AVDictionary **fltr_options)
{
    int error = 0;
    int ifmt_idx = -1;
    int istrm_idx = -1;
    int ostrm_idx = -1;
    int estrm_idx = -1;
    AVStream *dstrm = nullptr;
    AVStream *estrm = nullptr;
    AVCodecContext *dec_ctx = nullptr;
    AVCodecContext *enc_ctx = nullptr;
    std::string media_type;
    this->info_mgr->start_register_stream(estrm_idx);

    // get decode stream
    if ((error = this->info_mgr->stream_to_format(dstrm_idx,
                                                  ifmt_idx,
                                                  istrm_idx)) < 0)
    {
        goto __exit0;
    }

    if ((error = this->fmt_ctx_mgr->get_stream(ifmt_idx,
                                               istrm_idx,
                                               dstrm)) < 0)
    {
        goto __exit0;
    }

    // create decode stream packet queue
    if ((error = this->data_mgr->add_stream_packet_queue(dstrm_idx,
                                                         dstrm->time_base,
                                                         dstrm->time_base)) < 0)
    {
        goto __exit0;
    }

    // set transfer way
    if ((error = this->fmt_pkt_trfm_mgr->add_input_format_to_decode_stream_way(ifmt_idx,
                                                                               istrm_idx,
                                                                               dstrm_idx)) < 0)
    {
        goto __exit1;
    }

    // create decoder and get decoder context
    if ((error = this->codec_ctx_mgr->add_decoder_context(dstrm_idx,
                                                          dstrm)) < 0)
    {
        goto __exit2;
    }

    if ((error = this->codec_ctx_mgr->get_decoder_context(dstrm_idx,
                                                          dec_ctx)) < 0)
    {
        goto __exit3;
    }

    // create decode stream frame queue
    if ((error = this->data_mgr->add_stream_frame_queue(dstrm_idx,
                                                        dstrm->time_base,
                                                        dec_ctx->time_base)) < 0)
    {
        goto __exit3;
    }

    // parse encoder parameters
    if ((error = this->codec_parser->build_encoder_context(dstrm,
                                                           enc_info,
                                                           enc_ctx)) < 0)
    {
        goto __exit4;
    }

    // create encoder and get encoder context
    if ((error = this->codec_ctx_mgr->add_encoder_context(estrm_idx,
                                                          enc_ctx)) < 0)
    {
        goto __exit4;
    }

    if ((error = this->codec_ctx_mgr->get_encoder_context(estrm_idx,
                                                          enc_ctx)) < 0)
    {
        goto __exit5;
    }

    // add transformer
    if ((error = this->trfm_mgr->add_transformer(dstrm_idx,
                                                 estrm_idx,
                                                 dec_ctx,
                                                 enc_ctx)) < 0)
    {
        goto __exit5;
    }

    // create encode stream frame queue
    if ((error = this->data_mgr->add_stream_frame_queue(estrm_idx,
                                                        dec_ctx->time_base,
                                                        enc_ctx->time_base)) < 0)
    {
        goto __exit6;
    }

    // add encode stream to output format and get output stream
    if ((error = this->fmt_ctx_mgr->add_output_stream(ofmt_idx,
                                                      enc_ctx,
                                                      ostrm_idx)) < 0)
    {
        goto __exit7;
    }

    if ((error = this->fmt_ctx_mgr->get_stream(ofmt_idx,
                                               ostrm_idx,
                                               estrm)) < 0)
    {
        goto __exit8;
    }

    // create encode stream packet queue
    if ((error = this->data_mgr->add_stream_packet_queue(estrm_idx,
                                                         enc_ctx->time_base,
                                                         estrm->time_base)) < 0)
    {
        goto __exit8;
    }

    // set transfer way from encode stream to output format
    if ((error = this->fmt_pkt_trfm_mgr->add_encode_stream_to_output_format_way(estrm_idx,
                                                                                ofmt_idx,
                                                                                ostrm_idx)) < 0)
    {
        goto __exit9;
    }

    if (dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        media_type = "audio";
    }
    else if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        media_type = "video";
    }

    if ((error = this->info_mgr->set_stream_to_format_info(estrm_idx,
                                                           ofmt_idx,
                                                           ostrm_idx,
                                                           "encode",
                                                           media_type)) < 0)
    {
        goto __exit10;
    }

    goto __exit0;

__exit10:
    this->fmt_pkt_trfm_mgr->delete_encode_stream_to_output_format_way(ofmt_idx,
                                                                      estrm_idx);
__exit9:
    this->data_mgr->delete_stream_packet_queue(estrm_idx);
__exit8:
    this->fmt_ctx_mgr->delete_output_stream(ofmt_idx);
__exit7:
    this->data_mgr->delete_stream_frame_queue(estrm_idx);
__exit6:
    this->trfm_mgr->delete_transformer(dstrm_idx,
                                       estrm_idx);
__exit5:
    this->codec_ctx_mgr->delete_encoder_context(estrm_idx);
__exit4:
    this->data_mgr->delete_stream_frame_queue(dstrm_idx);
__exit3:
    this->codec_ctx_mgr->delete_decoder_context(dstrm_idx);
__exit2:
    this->fmt_pkt_trfm_mgr->delete_input_format_to_decode_stream_way(ifmt_idx,
                                                                     istrm_idx);
__exit1:
    this->data_mgr->delete_stream_packet_queue(dstrm_idx);
__exit0:
    if (error < 0)
    {
        this->info_mgr->cancel_register_stream();
        return error;
    }
    else
    {
        this->info_mgr->finish_register_stream();
        return estrm_idx;
    }
}

int AVCore::build_copy_pipeline(int dstrm_idx,
                                int ofmt_idx)
{
    int error = 0;
    int estrm_idx = -1;
    int ifmt_idx = -1;
    int istrm_idx = -1;
    AVStream *dstrm = nullptr;
    AVCodecContext *enc_ctx = nullptr;
    int ostrm_idx = -1;
    AVStream *estrm = nullptr;
    this->info_mgr->start_register_stream(estrm_idx);

    // get decode stream
    if ((error = this->info_mgr->stream_to_format(dstrm_idx,
                                                  ifmt_idx,
                                                  istrm_idx)) < 0)
    {
        goto __exit0;
    }

    if ((error = this->fmt_ctx_mgr->get_stream(ifmt_idx,
                                               istrm_idx,
                                               dstrm)) < 0)
    {
        goto __exit0;
    }

    // create encoder and get encoder context
    if ((error = this->codec_ctx_mgr->add_encoder_context(estrm_idx,
                                                          dstrm_idx,
                                                          dstrm)) < 0)
    {
        goto __exit0;
    }

    if ((error = this->codec_ctx_mgr->get_encoder_context(estrm_idx,
                                                          enc_ctx)) < 0)
    {
        goto __exit1;
    }

    // add stream to output format and get output stream
    if ((error = this->fmt_ctx_mgr->add_output_stream(ofmt_idx,
                                                      enc_ctx,
                                                      ostrm_idx)) < 0)
    {
        goto __exit1;
    }

    if ((error = this->fmt_ctx_mgr->get_stream(ofmt_idx,
                                               ostrm_idx,
                                               estrm)) < 0)
    {
        goto __exit1;
    }

    // create encode stream packet queue
    if ((error = this->data_mgr->add_stream_packet_queue(estrm_idx,
                                                         dstrm->time_base,
                                                         estrm->time_base)) < 0)
    {
        goto __exit1;
    }

    // set transfer way from input format to encode stream
    if ((error = this->fmt_pkt_trfm_mgr->add_input_format_to_encode_stream_way(ifmt_idx,
                                                                               istrm_idx,
                                                                               estrm_idx)) < 0)
    {
        goto __exit2;
    }

    // set transfer way from encode stream to output format
    if ((error = this->fmt_pkt_trfm_mgr->add_encode_stream_to_output_format_way(estrm_idx,
                                                                                ofmt_idx,
                                                                                ostrm_idx)) < 0)
    {
        goto __exit3;
    }

    goto __exit0;

__exit3:
    this->fmt_pkt_trfm_mgr->delete_input_format_to_encode_stream_way(ifmt_idx,
                                                                     istrm_idx,
                                                                     estrm_idx);
__exit2:
    this->data_mgr->delete_stream_packet_queue(estrm_idx);
__exit1:
    this->codec_ctx_mgr->delete_encoder_context(estrm_idx);
__exit0:
    if (error < 0)
    {
        this->info_mgr->cancel_register_stream();
        return error;
    }
    else
    {
        this->info_mgr->finish_register_stream();
        return estrm_idx;
    }
}

int AVCore::open_input(const std::string &url,
                       const AVInputFormat *fmt,
                       AVDictionary **options)
{
    int error = 0;

    // add input and register input format
    int ifmt_ctx_idx = -1;
    this->info_mgr->start_register_format(ifmt_ctx_idx);
    this->fmt_ctx_mgr->add_input_file(ifmt_ctx_idx,
                                      url,
                                      options);
    this->info_mgr->finish_register_format();
    AVFormatContext *ifmt_ctx = nullptr;
    this->fmt_ctx_mgr->get_format_context(ifmt_ctx_idx,
                                          ifmt_ctx);

    // create input format packet queue
    this->data_mgr->add_format_packet_queue(ifmt_ctx_idx,
                                            ifmt_ctx);

    // add decode streams and register decode streams
    for (int istrm_idx = 0;
         istrm_idx < ifmt_ctx->nb_streams;
         istrm_idx++)
    {
        AVStream *dec_strm = ifmt_ctx->streams[istrm_idx];

        if (dec_strm->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            int dec_strm_idx = -1;
            this->info_mgr->start_register_stream(dec_strm_idx);
            this->info_mgr->set_stream_to_format_info(dec_strm_idx,
                                                      ifmt_ctx_idx,
                                                      istrm_idx,
                                                      "decode",
                                                      "audio");
            this->info_mgr->finish_register_stream();
        }
        else if (dec_strm->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            int dec_strm_idx = -1;
            this->info_mgr->start_register_stream(dec_strm_idx);
            this->info_mgr->set_stream_to_format_info(dec_strm_idx,
                                                      ifmt_ctx_idx,
                                                      istrm_idx,
                                                      "decode",
                                                      "video");
            this->info_mgr->finish_register_stream();
        }
    }

    this->info_mgr->set_format_info(ifmt_ctx_idx,
                                    "input",
                                    url);

    return ifmt_ctx_idx;
}

int AVCore::open_output(const std::string &url,
                        const AVOutputFormat *fmt,
                        AVDictionary **options)
{
    int error = 0;

    // add output and register output format
    int ofmt_ctx_idx = -1;
    this->info_mgr->start_register_format(ofmt_ctx_idx);
    this->fmt_ctx_mgr->add_output_file(ofmt_ctx_idx,
                                       url,
                                       options);
    this->info_mgr->finish_register_format();
    AVFormatContext *ofmt_ctx = nullptr;
    this->fmt_ctx_mgr->get_format_context(ofmt_ctx_idx,
                                          ofmt_ctx);

    // create output format packet queue
    this->data_mgr->add_format_packet_queue(ofmt_ctx_idx,
                                            ofmt_ctx);

    this->info_mgr->set_format_info(ofmt_ctx_idx,
                                    "output",
                                    url);

    return ofmt_ctx_idx;
}

int AVCore::set_stream_link(nlohmann::json &input,
                            nlohmann::json &output)
{
    int error = 0;
    bool is_copy = input["copy"].get<bool>();
    int dstrm_idx = input["decode_stream_index"].get<int>();
    int ofmt_idx = input["output_format_index"].get<int>();

    if (is_copy)
    {
        if ((error = this->build_copy_pipeline(dstrm_idx,
                                               ofmt_idx)) < 0)
        {
            output["status"] = error;
            output["encode_stream_index"] = -1;
            return error;
        }
    }
    else
    {
        if ((error = this->build_decode_encode_pipeline(dstrm_idx,
                                                        ofmt_idx,
                                                        input["encoder_settings"],
                                                        nullptr)) < 0)
        {
            output["status"] = error;
            output["encode_stream_index"] = -1;
            return error;
        }
    }

    output["status"] = 0;
    output["encode_stream_index"] = error;
    return error;
}

int AVCore::get_streams_index_by_format_index(int fmt_ctx_idx,
                                              nlohmann::json &info)
{
    std::vector<int> streams_info;
    std::list<int> strm_idx_list = this->info_mgr->format_to_streams(fmt_ctx_idx);

    for (auto strm_idx : strm_idx_list)
    {
        streams_info.push_back(strm_idx);
    }

    info = streams_info;
    return 0;
}
