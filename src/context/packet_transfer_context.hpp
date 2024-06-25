#ifndef OSFF_PACKET_TRANSFER_CONTEXT_HPP
#define OSFF_PACKET_TRANSFER_CONTEXT_HPP

#include <unordered_map>
#include <vector>

class OSFFPacketTransferContext
{
public:
    int fmt_id = -1;

    int fmt_pkt_queue_id = -1;

    std::unordered_map<int, std::vector<int>> fmt2strm_queues_id;

    std::unordered_map<int, std::vector<int>> fmt2strm_way;

    std::unordered_map<int, int> strm2fmt_queues_id;

    std::unordered_map<int, int> strm2fmt_way;

public:
    OSFFPacketTransferContext(int fmt_id)
        : fmt_id(fmt_id) {}
};

#endif // OSFF_PACKET_TRANSFER_CONTEXT_HPP