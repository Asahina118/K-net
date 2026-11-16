#include "include/knet/reliable_udp/packet.h"

#include "src/detail/byte_utils.h"
#include <cstddef>

namespace knet::reliable_udp
{

    std::expected<std::vector<std::byte>, reliable_udp::Error>
        Serializer::serialize(const Packet& packet)
    {
        using namespace knet::detail;
        // TODO : return a std::vector<std::byte> as converted bytes
        // 1. check the length is equal to the payload length 
        // 2. turn everything into a byte stream (but how?)
        // 3. this function is only responsible for serializing, no need to know what the content is
        // this means it is protocol-agnostic

        std::vector<std::byte> bytes(PACKET_SIZE);
        size_t startIdx = 0;
        auto endian = byte_utils::Endian::BIG_ENDIAN;

        byte_utils::to_bytes(packet.header_.sequenceNumber_,
                endian,
                &bytes,
                startIdx
                );

        startIdx += sizeof(packet.header_.packetType_);
        byte_utils::to_bytes(packet.header_.packetType_,
                endian,
                &bytes,
                startIdx
                );

        startIdx += sizeof(packet.header_.sequenceNumber_);
        byte_utils::to_bytes(packet.header_.payloadLength_,
                endian,
                &bytes,
                startIdx
                );

        startIdx += sizeof(packet.header_.payloadLength_);

        /*
         * i need to know the type and length to do the big endian inversion tho...
         */

        // std::vector<std::byte> payload = packet.payload_;
        //
        // byte_utils::invert(&payload);
        // for (size_t i = startIdx; i < PACKET_SIZE; i++) {
        // }

        return {};
    }

    std::expected<Packet, reliable_udp::Error>
        Serializer::deserialize(std::span<const std::byte> payload)
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
