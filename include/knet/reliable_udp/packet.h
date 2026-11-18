#pragma once

#include <cstdint>
#include <expected>
#include <limits>
#include <span>
#include <vector>

#include "include/knet/reliable_udp/error.h"

namespace knet::reliable_udp::packet
{
    enum class PacketType : std::uint8_t
    {
        DATA = 0,
        ACK,
        SYN,
        FIN,
        UNKNOWN
    };

    PacketType parse_packet_type_from_byte(const std::byte byte);

    struct PacketHeader // 8 bytes after padding
    {
        std::uint32_t sequenceNumber_;
        std::uint16_t payloadLength_;
        PacketType packetType_;
        bool operator==(const PacketHeader& other) const = default;
    };

    struct Packet
    {
        PacketHeader header_;
        std::vector<std::byte> payload_;
        bool operator==(const Packet& other) const = default;
    };

    constexpr inline size_t PACKET_HEADER_SIZE = sizeof(PacketHeader);
    constexpr inline size_t PACKET_HEADER_SIZE_NO_PADDING = sizeof(PacketHeader) - 1;

    constexpr inline auto MAX_SEQUENCE_NUMBER = std::numeric_limits<decltype(PacketHeader::sequenceNumber_)>::max();
    constexpr inline auto MAX_SEQUENCE_PAYLOAD_LENGTH = std::numeric_limits<decltype(PacketHeader::payloadLength_)>::max();


    namespace serializer
    {
        std::expected<std::vector<std::byte>, reliable_udp::Error>
            serialize(const Packet& packet);

        // TODO : implement
        std::expected<Packet, reliable_udp::Error>
            deserialize(std::span<const std::byte> payload);


        // protocol :
        // packetType : 1 byte
        // sequenceNumber : 4 byte
        // payloadLength : 3 byte
        // payload: value of payloadLength
        constexpr inline size_t PACKET_PACKET_TYPE_OFFSET = 0;
        constexpr inline size_t PACKET_SEQUENCE_NUMBER_OFFSET = PACKET_PACKET_TYPE_OFFSET + sizeof(PacketType);
        constexpr inline size_t PACKET_PAYLOAD_LENGTH_OFFSET = sizeof(PacketHeader::sequenceNumber_) + PACKET_SEQUENCE_NUMBER_OFFSET;

        constexpr inline size_t PACKET_PAYLOAD_OFFSET = PACKET_HEADER_SIZE_NO_PADDING;
    }



}
