#ifndef SNAKE_EYE_TRANSFORMER_HPP
#define SNAKE_EYE_TRANSFORMER_HPP

/**
 * @file transformer.hpp
 * @author Winteryan (winteryan1998@gmail.com)
 * @brief Base class for data transformer, such as video scaler and audio resampler.
 */

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
}
#include <string>
#include <queue>

#include "../util/util.hpp"
#include "../static/macro.hpp"

namespace SnakeEye
{
    /**
     * @class SnakeEyeTransformer
     * @brief A base class for data transform.
     */
    class SnakeEyeTransformer
    {
    protected:
        enum AVMediaType media_type = AVMEDIA_TYPE_UNKNOWN;

        int dstrm_idx = -1;

        int estrm_idx = -1;

        std::string strm_map_info;

        const AVCodecContext *dec_ctx = nullptr;

        const AVCodecContext *enc_ctx = nullptr;

        std::queue<AVFrame *> frm_cache;

        int status = 0;

    public:
        /**
         * @brief Construct a new Transformer object
         * @param media_type The media type of this transformer
         * @param dec_ctx The decoder context
         * @param enc_ctx The encoder context
         */
        SnakeEyeTransformer(enum AVMediaType media_type,
                            int dstrm_idx,
                            int estrm_idx,
                            const AVCodecContext *dec_ctx,
                            const AVCodecContext *enc_ctx)
            : media_type(media_type),
              dstrm_idx(dstrm_idx),
              estrm_idx(estrm_idx),
              dec_ctx(dec_ctx),
              enc_ctx(enc_ctx)
        {
            this->strm_map_info = "decoding stream #" +
                                  std::to_string(this->dstrm_idx) +
                                  " and encoding stream #" +
                                  std::to_string(this->estrm_idx);
        };

        /**
         * @brief Destroy the Transformers object
         */
        virtual ~SnakeEyeTransformer(){};

        int get_status()
        {
            return this->status;
        }

        /**
         * @brief Transform data from `src` to `dst`
         * @param src_buf The source data FIFO buffer
         * @param dst_buf The destination data FIFO buffer
         * @return non-negative number of data transformed (0 if no data transformed),
         * or negative AVERROR code on errors
         */
        virtual int send_frame(AVFrame *dec_frm) = 0;

        int receive_frame(AVFrame *&enc_frm)
        {
            if (this->frm_cache.empty())
            {
                return AVERROR(EAGAIN);
            }
            else
            {
                enc_frm = this->frm_cache.front();
                this->frm_cache.pop();

                if (enc_frm == nullptr)
                {
                    return AVERROR_EOF;
                }
                else
                {
                    return 0;
                }
            }
        }
    };
}

#endif // SNAKE_EYE_TRANSFORMER_HPP
