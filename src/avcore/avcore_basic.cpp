#include "avcore.h"

void AVCore::init()
{
    // initialize data manager
    this->data_mgr = new DataManager();

    // make data callback function
    auto push_cb = std::bind(&DataManager::push_data,
                             this->data_mgr,
                             std::placeholders::_1,
                             std::placeholders::_2,
                             std::placeholders::_3,
                             std::placeholders::_4);
    auto pop_cb = std::bind(&DataManager::pop_data,
                            this->data_mgr,
                            std::placeholders::_1,
                            std::placeholders::_2,
                            std::placeholders::_3,
                            std::placeholders::_4);

    // initialize info manager
    this->info_mgr = new InfoManager();

    // initialize format context manager and set corresponding callback functions
    this->fmt_ctx_mgr = new FormatContextManager(push_fmt_pkt_cb_func,
                                                 pop_fmt_pkt_cb_func);

    // initialize format packet transfer manager and set corresponding callback functions
    this->fmt_pkt_trfm_mgr = new FormatPacketTransferManager(pop_fmt_pkt_cb_func,
                                                             push_strm_pkt_cb_func,
                                                             pop_strm_pkt_cb_func,
                                                             push_fmt_pkt_cb_func);

    // initialize codec context manager and set corresponding callback functions
    this->codec_ctx_mgr = new CodecContextManager(pop_strm_pkt_cb_func,
                                                  push_strm_frm_cb_func,
                                                  pop_strm_frm_cb_func,
                                                  push_strm_pkt_cb_func);

    // initialize transformer manager and set corresponding callback functions
    this->trfm_mgr = new TransformerManager(pop_strm_frm_cb_func,
                                            push_strm_frm_cb_func);

    // initialize play manager and set corresponding callback functions
    this->play_mgr = new PlayManager(pop_strm_frm_cb_func,
                                     std::bind(&DataManager::push_play_stream_frame,
                                               this->data_mgr,
                                               std::placeholders::_1,
                                               std::placeholders::_2,
                                               std::placeholders::_3));

    // initialize codec parser
    this->codec_parser = new OSFFCodecParser();
}

void AVCore::clear()
{
    delete this->fmt_ctx_mgr;
    this->fmt_ctx_mgr = nullptr;
    delete this->fmt_pkt_trfm_mgr;
    this->fmt_pkt_trfm_mgr = nullptr;
    delete this->codec_ctx_mgr;
    this->codec_ctx_mgr = nullptr;
    delete this->trfm_mgr;
    this->trfm_mgr = nullptr;
    delete this->data_mgr;
    this->data_mgr = nullptr;
}

int AVCore::start()
{
    this->data_mgr->start();
    this->fmt_ctx_mgr->start();
    this->fmt_pkt_trfm_mgr->start();
    this->codec_ctx_mgr->start();
    this->trfm_mgr->start();
    this->play_mgr->start();
    return 0;
}