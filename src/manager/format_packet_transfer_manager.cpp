#include "format_packet_transfer_manager.h"

FormatPacketTransferManager::~FormatPacketTransferManager()
{
    for (auto it = this->read_transfer_thread_ctxs.begin();
         it != this->read_transfer_thread_ctxs.end();
         it++)
    {
        delete it->second;
    }

    this->read_transfer_thread_ctxs.clear();

    for (auto it = this->write_transfer_thread_ctxs.begin();
         it != this->write_transfer_thread_ctxs.end();
         it++)
    {
        delete it->second;
    }

    this->write_transfer_thread_ctxs.clear();
}

int FormatPacketTransferManager::add_input_format_to_decode_stream_way(int fmt_ctx_idx,
                                                                       int in_fmt_istrm_idx,
                                                                       int istrm_idx)
{
    auto it = this->read_transfer_thread_ctxs.find(fmt_ctx_idx);

    if (it == this->read_transfer_thread_ctxs.end())
    {
        this->read_transfer_thread_ctxs[fmt_ctx_idx] = new OSFFReadTransferThreadContext(fmt_ctx_idx,
                                                                                         this->pop_fmt_pkt_cb_func,
                                                                                         this->push_strm_pkt_cb_func);
    }

    this->read_transfer_thread_ctxs[fmt_ctx_idx]->add_decode_stream_way(in_fmt_istrm_idx,
                                                                        istrm_idx);
    return 0;
}

int FormatPacketTransferManager::delete_input_format_to_decode_stream_way(int fmt_ctx_idx, int in_fmt_istrm_idx)
{
    auto it = this->read_transfer_thread_ctxs.find(fmt_ctx_idx);

    if (it == this->read_transfer_thread_ctxs.end())
    {
        return AVERROR(EINVAL);
    }

    this->read_transfer_thread_ctxs[fmt_ctx_idx]->delete_decode_stream_way(in_fmt_istrm_idx);
    return 0;
}

int FormatPacketTransferManager::add_input_format_to_encode_stream_way(int fmt_ctx_idx, int in_fmt_istrm_idx, int ostrm_idx)
{
    auto it = this->read_transfer_thread_ctxs.find(fmt_ctx_idx);

    if (it == this->read_transfer_thread_ctxs.end())
    {
        this->read_transfer_thread_ctxs[fmt_ctx_idx] = new OSFFReadTransferThreadContext(fmt_ctx_idx,
                                                                                         this->pop_fmt_pkt_cb_func,
                                                                                         this->push_strm_pkt_cb_func);
    }

    this->read_transfer_thread_ctxs[fmt_ctx_idx]->add_encode_stream_way(in_fmt_istrm_idx,
                                                                        ostrm_idx);
    return 0;
}

int FormatPacketTransferManager::delete_input_format_to_encode_stream_way(int fmt_ctx_idx, int in_fmt_istrm_idx, int ostrm_idx)
{
    auto it = this->read_transfer_thread_ctxs.find(fmt_ctx_idx);

    if (it == this->read_transfer_thread_ctxs.end())
    {
        return AVERROR(EINVAL);
    }

    this->read_transfer_thread_ctxs[fmt_ctx_idx]->delete_encode_stream_way(in_fmt_istrm_idx,
                                                                           ostrm_idx);
    return 0;
}

int FormatPacketTransferManager::add_encode_stream_to_output_format_way(int ostrm_idx,
                                                                        int fmt_ctx_idx,
                                                                        int in_fmt_ostrm_idx)
{
    auto it = this->write_transfer_thread_ctxs.find(fmt_ctx_idx);

    if (it == this->write_transfer_thread_ctxs.end())
    {
        this->write_transfer_thread_ctxs[fmt_ctx_idx] = new OSFFWriteTransferThreadContext(fmt_ctx_idx,
                                                                                           this->pop_strm_pkt_cb_func,
                                                                                           this->push_fmt_pkt_cb_func);
    }

    this->write_transfer_thread_ctxs[fmt_ctx_idx]->add_output_stream_way(ostrm_idx,
                                                                         in_fmt_ostrm_idx);
    return 0;
}

int FormatPacketTransferManager::delete_encode_stream_to_output_format_way(int fmt_ctx_idx, int ostrm_idx)
{
    auto it = this->write_transfer_thread_ctxs.find(fmt_ctx_idx);

    if (it == this->write_transfer_thread_ctxs.end())
    {
        return AVERROR(EINVAL);
    }

    this->write_transfer_thread_ctxs[fmt_ctx_idx]->delete_output_stream_way(ostrm_idx);
    return 0;
}

int FormatPacketTransferManager::start()
{
    for (auto it = this->read_transfer_thread_ctxs.begin();
         it != this->read_transfer_thread_ctxs.end();
         it++)
    {
        it->second->start();
    }

    for (auto it = this->write_transfer_thread_ctxs.begin();
         it != this->write_transfer_thread_ctxs.end();
         it++)
    {
        it->second->start();
    }

    return 0;
}
