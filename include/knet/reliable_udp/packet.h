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

    struct PacketHeader
    {
        PacketType packetType_;
        std::uint32_t sequenceNumber_;
        std::uint16_t payloadLength_;
    };

    struct Packet
    {
        PacketHeader header_;
        std::vector<std::byte> payload_;
    };

    std::expected<std::vector<std::byte>, reliable_udp::Error>
        serialize(const Packet& packet);

    std::expected<Packet, reliable_udp::Error>
        deserialize(std::span<const std::byte> payload);
}
