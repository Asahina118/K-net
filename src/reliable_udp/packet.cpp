#include "include/knet/reliable_udp/packet.h"

#include "src/detail/utils/byte_utils.h"
#include <cstddef>

namespace knet::reliable_udp::packet
{

    /*
     * maybe use the avro format in the future?
     */
    std::expected<std::vector<std::byte>, reliable_udp::Error>
        serializer::serialize(const Packet& packet)
    {
        using namespace knet::detail;

        const size_t BYTE_SIZE = PACKET_HEADER_SIZE_NO_PADDING + packet.header_.payloadLength_;
        std::println("payloadLength_={}", packet.header_.payloadLength_);
        std::println("sizeof(PacketHeader)={}", PACKET_HEADER_SIZE_NO_PADDING);
        std::println("BYTE_SIZE={}", BYTE_SIZE);
        std::vector<std::byte> bytes(BYTE_SIZE);
        size_t startIdx = 0;
        auto endian = byte_utils::Endian::Big;


        byte_utils::to_bytes(packet.header_.packetType_,
                endian,
                &bytes,
                startIdx
                );

        startIdx += sizeof(packet.header_.packetType_);
        byte_utils::to_bytes(packet.header_.sequenceNumber_,
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
        for (size_t i = startIdx; i < startIdx + packet.header_.payloadLength_; i++) {
            bytes[i] = packet.payload_[i - startIdx];
        }

        return bytes;
    }

    std::expected<Packet, reliable_udp::Error>
        serializer::deserialize(std::span<const std::byte> payload)
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
