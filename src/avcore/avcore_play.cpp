#include "avcore.h"

int AVCore::link_audio_play(int dec_strm_idx)
{
    int error = 0;
    int enc_strm_idx = AUDIO_PLAY_STREAM_INDEX;

    // get input stream index
    int ifmt_ctx_idx = -1;
    int istrm_idx = -1;
    this->info_mgr->stream_to_format(dec_strm_idx,
                                     ifmt_ctx_idx,
                                     istrm_idx);

    // get input stream
    AVStream *istrm = nullptr;
    this->fmt_ctx_mgr->get_stream(ifmt_ctx_idx,
                                  istrm_idx,
                                  istrm);

    // add decode stream
    this->data_mgr->add_stream_packet_queue(dec_strm_idx,
                                            istrm->time_base,
                                            istrm->time_base);
    this->fmt_pkt_trfm_mgr->add_input_format_to_decode_stream_way(ifmt_ctx_idx,
                                                                  istrm_idx,
                                                                  dec_strm_idx);
    this->codec_ctx_mgr->add_decoder_context(dec_strm_idx,
                                             istrm);
    AVCodecContext *dec_ctx = nullptr;
    this->codec_ctx_mgr->get_decoder_context(dec_strm_idx,
                                             dec_ctx);
    this->data_mgr->add_stream_frame_queue(dec_strm_idx,
                                           istrm->time_base,
                                           dec_ctx->time_base);

    // add encode stream (no packet queue)
    this->data_mgr->add_stream_frame_queue(enc_strm_idx,
                                           dec_ctx->time_base,
                                           dec_ctx->time_base);

    // add transformer
    this->trfm_mgr->add_audio_play_transformer(dec_strm_idx,
                                               dec_ctx);

    // add play stream
    this->play_mgr->set_audio_play();
    this->data_mgr->add_play_stream_queue(AUDIO_PLAY_STREAM_INDEX,
                                          dec_ctx->time_base);

    return error;
}

int AVCore::link_video_play(int dec_strm_idx)
{
    int error = 0;
    int enc_strm_idx = VIDEO_PLAY_STREAM_INDEX;

    // get input stream index
    int ifmt_ctx_idx = -1;
    int istrm_idx = -1;
    this->info_mgr->stream_to_format(dec_strm_idx,
                                     ifmt_ctx_idx,
                                     istrm_idx);

    // get input stream
    AVStream *istrm = nullptr;
    this->fmt_ctx_mgr->get_stream(ifmt_ctx_idx,
                                  istrm_idx,
                                  istrm);

    // add decode stream
    this->data_mgr->add_stream_packet_queue(dec_strm_idx,
                                            istrm->time_base,
                                            istrm->time_base);
    this->fmt_pkt_trfm_mgr->add_input_format_to_decode_stream_way(ifmt_ctx_idx,
                                                                  istrm_idx,
                                                                  dec_strm_idx);
    this->codec_ctx_mgr->add_decoder_context(dec_strm_idx,
                                             istrm);
    AVCodecContext *dec_ctx = nullptr;
    this->codec_ctx_mgr->get_decoder_context(dec_strm_idx,
                                             dec_ctx);
    this->data_mgr->add_stream_frame_queue(dec_strm_idx,
                                           istrm->time_base,
                                           dec_ctx->time_base);

    // add encode stream (no packet queue)
    this->data_mgr->add_stream_frame_queue(enc_strm_idx,
                                           dec_ctx->time_base,
                                           dec_ctx->time_base);

    // add transformer
    this->trfm_mgr->add_video_play_transformer(dec_strm_idx,
                                               dec_ctx);

    // add play stream
    this->play_mgr->set_video_play();
    this->data_mgr->add_play_stream_queue(VIDEO_PLAY_STREAM_INDEX,
                                          dec_ctx->time_base);

    return error;
}

int AVCore::peek_play_frame(int play_strm_idx,
                            PlayFrame *&frm)
{
    return this->data_mgr->peek_play_stream_frame(play_strm_idx,
                                                  frm);
}

int AVCore::peek_last_play_frame(int play_strm_idx, PlayFrame *&frm)
{
    return this->data_mgr->peek_last_play_stream_frame(play_strm_idx,
                                                       frm);
}

int AVCore::peek_next_play_frame(int play_strm_idx, PlayFrame *&frm)
{
    return this->data_mgr->peek_next_play_stream_frame(play_strm_idx,
                                                       frm);
}

int AVCore::play_frame_size(int play_strm_idx)
{
    return this->data_mgr->play_stream_queue_size(play_strm_idx);
}

int AVCore::pop_play_stream_frame(int play_strm_idx)
{
    return this->data_mgr->pop_play_stream_frame(play_strm_idx);
}

int AVCore::set_clock_at(int play_strm_idx,
                         double pts,
                         double time)
{
    return this->play_mgr->set_clock_at(play_strm_idx,
                                        pts,
                                        time);
}

int AVCore::set_clock(int play_strm_idx,
                      double pts)
{
    return this->play_mgr->set_clock(play_strm_idx,
                                     pts);
}

int AVCore::get_clock(int play_strm_idx,
                      double &get_time)
{
    return this->play_mgr->get_clock(play_strm_idx,
                                     get_time);
}

int AVCore::set_frame_clock(int play_strm_idx,
                            double time)
{
    return this->play_mgr->set_frame_clock(play_strm_idx,
                                           time);
}

int AVCore::get_frame_clock(int play_strm_idx,
                            double &time)
{
    return this->play_mgr->get_frame_clock(play_strm_idx,
                                           time);
}