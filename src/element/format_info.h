#ifndef OSFF_FORMAT_INFO_H
#define OSFF_FORMAT_INFO_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include <string>
#include <vector>

class OSFFFormatInfo
{
public:
    int fmt_idx = -1;

    std::vector<int> strm_idxs;

    std::string io_type;

    std::string url;

    OSFFFormatInfo(int fmt_idx,
                   std::string io_type,
                   std::string url)
    {
        this->fmt_idx = fmt_idx;
        this->io_type = io_type;
        this->url = url;
    }
};

#endif // OSFF_FORMAT_INFO_H