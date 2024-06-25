#ifndef OSFF_STREAM_ID_GENERATOR_HPP
#define OSFF_STREAM_ID_GENERATOR_HPP

extern "C"
{
#include <libavutil/error.h>
}

#include <mutex>

class OSFFStreamIDGenerator
{
private:
    std::mutex mtx;

    std::vector<bool> id_map;

    int max_size = 0;

    int cur_size = 0;

    OSFFStreamIDGenerator()
    {
        this->id_map.resize(16, false);
        this->max_size = 16;
        this->cur_size = 0;
    }

    OSFFStreamIDGenerator(const OSFFStreamIDGenerator &) = delete;

    OSFFStreamIDGenerator &operator=(const OSFFStreamIDGenerator &) = delete;

public:
    static OSFFStreamIDGenerator &get_instance()
    {
        static OSFFStreamIDGenerator instance;
        return instance;
    }

    int registe_id(int &id)
    {
        std::lock_guard<std::mutex> lock(this->mtx);

        if (this->cur_size < this->max_size)
        {
            for (int i = 0; i < this->max_size; i++)
            {
                if (!this->id_map[i])
                {
                    this->id_map[i] = true;
                    id = i;
                    this->cur_size++;
                    return 0;
                }
            }
        }
        else
        {
            this->id_map.push_back(true);
            this->max_size++;
            id = this->max_size - 1;
            this->cur_size++;
            return 0;
        }

        return 0;
    }

    int release_id(int id)
    {
        std::lock_guard<std::mutex> lock(this->mtx);

        if (id < 0 || id >= this->max_size)
        {
            return AVERROR(EINVAL);
        }

        if (this->id_map[id])
        {
            this->id_map[id] = false;
            this->cur_size--;
        }

        return 0;
    }
};

#endif // OSFF_STREAM_ID_GENERATOR_HPP