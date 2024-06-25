#ifndef FORMAT_PACKET_TRANSFER_MANAGER_H
#define FORMAT_PACKET_TRANSFER_MANAGER_H

#include <unordered_map>
#include <functional>

#include "../thread/read_transfer_thread.h"
#include "../thread/write_transfer_thread.h"

class FormatPacketTransferManager
{
private:
    std::unordered_map<int, OSFFReadTransferThreadContext *> read_transfer_thread_ctxs;

    std::unordered_map<int, OSFFWriteTransferThreadContext *> write_transfer_thread_ctxs;

    std::function<int(int, AVPacket *&, int)> pop_fmt_pkt_cb_func;

    std::function<int(int, AVPacket *, int)> push_strm_pkt_cb_func;

    std::function<int(int, AVPacket *&, int)> pop_strm_pkt_cb_func;

    std::function<int(int, AVPacket *, int)> push_fmt_pkt_cb_func;

public:
    FormatPacketTransferManager(std::function<int(int, AVPacket *&, int)> pop_fmt_pkt_cb_func,
                                std::function<int(int, AVPacket *, int)> push_strm_pkt_cb_func,
                                std::function<int(int, AVPacket *&, int)> pop_strm_pkt_cb_func,
                                std::function<int(int, AVPacket *, int)> push_fmt_pkt_cb_func)
        : pop_fmt_pkt_cb_func(pop_fmt_pkt_cb_func),
          push_strm_pkt_cb_func(push_strm_pkt_cb_func),
          pop_strm_pkt_cb_func(pop_strm_pkt_cb_func),
          push_fmt_pkt_cb_func(push_fmt_pkt_cb_func){};

    ~FormatPacketTransferManager();

    int add_input_format_to_decode_stream_way(int fmt_ctx_idx,
                                              int in_fmt_istrm_idx,
                                              int istrm_idx);

    int delete_input_format_to_decode_stream_way(int fmt_ctx_idx,
                                                 int in_fmt_istrm_idx);

    int add_input_format_to_encode_stream_way(int fmt_ctx_idx,
                                              int in_fmt_istrm_idx,
                                              int ostrm_idx);

    int delete_input_format_to_encode_stream_way(int fmt_ctx_idx,
                                                 int in_fmt_istrm_idx,
                                                 int ostrm_idx);

    int add_encode_stream_to_output_format_way(int ostrm_idx,
                                               int fmt_ctx_idx,
                                               int in_fmt_ostrm_idx);

    int delete_encode_stream_to_output_format_way(int fmt_ctx_idx,
                                                  int ostrm_idx);

    int start();
};

#endif // FORMAT_PACKET_TRANSFER_MANAGER_H
