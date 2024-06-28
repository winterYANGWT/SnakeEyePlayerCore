#ifndef CODEC_CONTEXT_MANAGER_H
#define CODEC_CONTEXT_MANAGER_H

/**
 * @file codec_context_manager.h
 * @brief CodecContextManager class
 */

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <string>
#include <unordered_map>

#include "../util/util.h"
#include "../thread/decode_thread.h"
#include "../thread/encode_thread.h"
#include "../context/stream_context.hpp"

/**
 * @brief CodecContextManager used to manage a list of codec contexts.
 * It can automatically create codec contexts from a format context which has streams.
 * it can also be created from a list of codec contexts.
 * When it is destructed, it will free all the codec contexts it manages
 */
class CodecContextManager
{
private:
    std::unordered_map<int, OSFFStreamContext *> strm_ctxs;

    std::unordered_map<int, OSFFDecodeThreadContext *> dec_thread_ctxs;

    std::unordered_map<int, OSFFEncodeThreadContext *> enc_thread_ctxs;

    std::unordered_map<int, int> ctxs_ref_count;

    std::function<int(int, AVPacket *&, int)> pop_pkt_cb_func;

    std::function<int(int, AVFrame *, int)> push_frm_cb_func;

    std::function<int(int, AVFrame *&, int)> pop_frm_cb_func;

    std::function<int(int, AVPacket *, int)> push_pkt_cb_func;

public:
    CodecContextManager(std::function<int(int, AVPacket *&, int)> pop_pkt_cb_func,
                        std::function<int(int, AVFrame *, int)> push_frm_cb_func,
                        std::function<int(int, AVFrame *&, int)> pop_frm_cb_func,
                        std::function<int(int, AVPacket *, int)> push_pkt_cb_func)
        : pop_pkt_cb_func(pop_pkt_cb_func),
          push_frm_cb_func(push_frm_cb_func),
          pop_frm_cb_func(pop_frm_cb_func),
          push_pkt_cb_func(push_pkt_cb_func) {}

    ~CodecContextManager();

    int add_encoder_context(int strm_idx,
                            AVCodecContext *enc_ctx)
    {
        auto it = this->ctxs_ref_count.find(strm_idx);

        if (it == this->ctxs_ref_count.end() ||
            it->second == 0)
        {
            this->enc_thread_ctxs[strm_idx] = new OSFFEncodeThreadContext(strm_idx,
                                                                          enc_ctx,
                                                                          this->pop_frm_cb_func,
                                                                          this->push_pkt_cb_func);
            this->ctxs_ref_count[strm_idx] = 1;
        }
        else
        {
            this->ctxs_ref_count[strm_idx]++;
        }

        return 0;
    }

    int add_encoder_context(int enc_strm_idx,
                            int dec_strm_idx,
                            AVStream *dec_strm);

    int get_encoder_context(int strm_idx,
                            AVCodecContext *&enc_ctx);

    int delete_encoder_context(int enc_strm_idx);

    int add_decoder_context(int strm_idx,
                            AVStream *strm);

    int get_decoder_context(int strm_idx,
                            AVCodecContext *&dec_ctx);

    int delete_decoder_context(int strm_idx);

    int start();
};

#endif // CODEC_CONTEXT_MANAGER_H
