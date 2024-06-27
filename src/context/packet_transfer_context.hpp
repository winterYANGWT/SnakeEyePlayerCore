#ifndef SNAKE_EYE_PACKET_TRANSFER_CONTEXT_HPP
#define SNAKE_EYE_PACKET_TRANSFER_CONTEXT_HPP

#include <unordered_map>
#include <vector>

namespace SnakeEye
{
    class SnakeEyePacketTransferContext
    {
    public:
        int fmt_id = -1;

        int fmt_pkt_queue_id = -1;

        std::unordered_map<int, std::vector<int>> fmt2strm_queues_id;

        std::unordered_map<int, std::vector<int>> fmt2strm_way;

        std::unordered_map<int, int> strm2fmt_queues_id;

        std::unordered_map<int, int> strm2fmt_way;

    public:
        SnakeEyePacketTransferContext(int fmt_id)
            : fmt_id(fmt_id) {}
    };
}

#endif // SNAKE_EYE_PACKET_TRANSFER_CONTEXT_HPP
