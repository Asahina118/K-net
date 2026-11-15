#include "include/knet/reliable_udp/packet.h"

namespace knet::reliable_udp
{
    std::expected<std::vector<std::byte>, reliable_udp::Error>
        serialize(const Packet& packet)
    {
        // TODO : return a std::vector<std::byte> as converted bytes
        // 1. check the length is equal to the payload length 
        // 2. turn everything into a byte stream (but how?)
        return {};
    }

    std::expected<Packet, reliable_udp::Error>
        deserialize(std::span<const std::byte> payload)
    {
        // TODO:: need to consider the truncated sceanario
        // 1. the length is currently fixed, the first 7 bytes followed by the payload, truncated size depends on the buffer size of the recvmsg api
        //

        /* use case for (reliable udp implementation?): 
         *
         * ReceivedResult result = recvmsg(endpoint);
         * log("received " + result.buffer_ + " raw bytes");
         * Packet packet = deserialize(result->buffer_);
         * if (packet) {
         *   reliable mechanisms: keep track of the sequence number (in where? in the UDPSocket?)
         * }
         */
        return {};
    }
}
