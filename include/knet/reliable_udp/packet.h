#pragma once

#include <cstdint>
#include <expected>

#include <span>
#include <vector>

#include "include/knet/reliable_udp/error.h"

namespace knet::reliable_udp
{
    enum class PacketType : std::uint8_t
    {
        DATA = 0,
        ACK,
        SYN,
        FIN
    };

    struct PacketHeader // 8 bytes after padding
    {
        std::uint32_t sequenceNumber_;
        std::uint16_t payloadLength_;
        PacketType packetType_;
    };

    struct Packet
    {
        PacketHeader header_;
        std::vector<std::byte> payload_;
    };

    // TODO : implement
    std::expected<std::vector<std::byte>, reliable_udp::Error>
        serialize(const Packet& packet);

    // TODO : implement
    std::expected<Packet, reliable_udp::Error>
        deserialize(std::span<const std::byte> payload);
}
