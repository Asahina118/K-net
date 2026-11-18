#include <cstddef>
#include <print>

#include "tests/extern/doctest.h"
#include "include/knet/reliable_udp/packet.h"
#include "src/detail/utils/byte_utils.h"

TEST_CASE("packet serializer test")
{
    using namespace knet::reliable_udp::packet;
    using namespace knet::detail::byte_utils;

    Packet packetTest = Packet {
        .header_ = PacketHeader {
            .sequenceNumber_ = 1,
            .payloadLength_ = 5,
            .packetType_ = PacketType::DATA
        },
        .payload_ = std::vector<std::byte>(5, std::byte{0xFF})
    };

    std::println("sequenceNumber_ in byte={:02X}", packetTest.header_.sequenceNumber_);
    std::println("payloadLength_ in byte={:02X}", packetTest.header_.payloadLength_);
    std::println("packetType_ in byte={:02X}", static_cast<unsigned int>(packetTest.header_.packetType_));


    auto result = serializer::serialize(packetTest);

    if (!result) {
        std::println("error occurred when serializing");
    }
    
    std::println("serialization success!");
    for (const std::byte byte : *result) {
        std::print("{:02X}, ", std::to_integer<unsigned int>(byte));
    }
    std::println();

    // std::println("converting packetType_ to byte");
    // std::println("\nconverting sequenceNumber_ to byte");
    // std::println("\nconverting payloadLength_ to byte");
    // std::println("converting packetType_ to byte");
    // byte_utils::to_bytes(packet.header_.packetType_,
    //         endian,
    //         &bytes,
    //         startIdx
    //         );
    //
    // std::println("\nconverting sequenceNumber_ to byte");
    // startIdx += sizeof(packet.header_.packetType_);
    // byte_utils::to_bytes(packet.header_.sequenceNumber_,
    //         endian,
    //         &bytes,
    //         startIdx
    //         );
    //
    //
    // std::println("\nconverting payloadLength_ to byte");
    // startIdx += sizeof(packet.header_.sequenceNumber_);
    // byte_utils::to_bytes(packet.header_.payloadLength_,
    //         endian,
    //         &bytes,
    //         startIdx
    //         );
}


