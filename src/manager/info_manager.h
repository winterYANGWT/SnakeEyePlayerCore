#ifndef INFO_MANAGER_H
#define INFO_MANAGER_H

#include <mutex>
#include <unordered_map>
#include <map>
#include <list>
#include <nlohmann/json.hpp>

#include "../element/format_info.h"
#include "../element/stream_info.h"

class InfoManager
{
private:
    int fmt_count = 0;

    int strm_count = 0;

    std::map<int, OSFFStreamInfo *> strm_info_map;

    std::map<int, OSFFFormatInfo *> fmt_info_map;

    std::unordered_map<int, std::list<int>> fmt2strm_map;

    std::unordered_map<int, int> strm2fmt_map;

    std::mutex fmt_count_mtx;

    std::mutex strm_count_mtx;

    int f2s_hash(int ifmt_idx,
                 int istrm_idx)
    {
        return ifmt_idx * 0x10000 + istrm_idx;
    }

public:
    void start_register_format(int &fmt_idx)
    {
        this->fmt_count_mtx.lock();
        fmt_idx = this->fmt_count;
    }

    void finish_register_format()
    {

        this->fmt2strm_map[this->fmt_count++] = std::list<int>();
        this->fmt_count_mtx.unlock();
    }

    void cancel_register_format()
    {
        this->fmt_count_mtx.unlock();
    }

    void start_register_stream(int &strm_idx)
    {
        this->strm_count_mtx.lock();
        strm_idx = this->strm_count;
    }

    void finish_register_stream()
    {
        this->strm_count++;
        this->strm_count_mtx.unlock();
    }

    void cancel_register_stream()
    {
        this->strm_count_mtx.unlock();
    }

    int set_stream_to_format_info(int strm_idx,
                                  int fmt_idx,
                                  int in_fmt_strm_idx,
                                  std::string code_type,
                                  std::string media_type)
    {
        this->strm_info_map[strm_idx] = new OSFFStreamInfo(strm_idx,
                                                           fmt_idx,
                                                           in_fmt_strm_idx,
                                                           code_type,
                                                           media_type);
        this->fmt2strm_map[fmt_idx].push_back(strm_idx);
        return 0;
    }

    int set_format_info(int fmt_idx,
                        std::string io_type,
                        std::string url)
    {
        this->fmt_info_map[fmt_idx] = new OSFFFormatInfo(fmt_idx,
                                                         io_type,
                                                         url);
        return 0;
    }

    int stream_to_format(int strm_idx,
                         int &fmt_idx,
                         int &in_fmt_strm_idx)
    {
        auto it = this->strm_info_map.find(strm_idx);

        if (it == this->strm_info_map.end())
        {
            return AVERROR(EINVAL);
        }
        else
        {
            fmt_idx = it->second->fmt_idx;
            in_fmt_strm_idx = it->second->in_fmt_strm_idx;
            return 0;
        }
    }

    std::list<int> format_to_streams(int fmt_idx)
    {
        auto it = this->fmt2strm_map.find(fmt_idx);

        if (it == this->fmt2strm_map.end())
        {
            return std::list<int>();
        }
        else
        {
            return it->second;
        }
    }

    int get_output_formats(std::vector<OSFFFormatInfo *> &formats)
    {
        formats.clear();

        for (auto it = this->fmt_info_map.begin();
             it != this->fmt_info_map.end();
             it++)
        {
            if (it->second->io_type == "output")
            {
                formats.push_back(it->second);
            }
        }

        return 0;
    }

    int get_decode_streams(std::vector<OSFFStreamInfo *> &streams)
    {
        streams.clear();

        for (auto it = this->strm_info_map.begin();
             it != this->strm_info_map.end();
             it++)
        {
            if (it->second->code_type == "decode")
            {
                streams.push_back(it->second);
            }
        }

        return 0;
    }

    int get_stream_info(int strm_idx,
                        OSFFStreamInfo *&stream_info)
    {
        auto it = this->strm_info_map.find(strm_idx);

        if (it == this->strm_info_map.end())
        {
            return AVERROR(EINVAL);
        }
        else
        {
            stream_info = it->second;
            return 0;
        }
    }
};

#endif // INFO_MANAGER_H
