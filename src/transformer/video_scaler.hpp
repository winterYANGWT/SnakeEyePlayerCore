#ifndef OSFF_VIDEO_SCALER_HPP
#define OSFF_VIDEO_SCALER_HPP

extern "C"
{
#include <libswscale/swscale.h>
}

#include "transformer.hpp"

class OSFFVideoScaler : public OSFFTransformer
{
private:
    SwsContext *sws_ctx = nullptr;

public:
    OSFFVideoScaler(int dstrm_idx,
                    int estrm_idx,
                    const AVCodecContext *dec_ctx,
                    const AVCodecContext *enc_ctx);

    ~OSFFVideoScaler()
    {
        sws_freeContext(this->sws_ctx);
    };

    // virtual int transform(AVFrame *dec_frm,
    //                       AVFrame **enc_frm);

    virtual int send_frame(AVFrame *dec_frm);
};

#endif // OSFF_VIDEO_SCALER_HPP
