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
}

TEST_CASE("packet deserializer test")
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

     auto result = serializer::serialize(packetTest);
     auto resultDeserialized = serializer::deserialize(*result);
     Packet deserializedPacket = *resultDeserialized;
     // CHECK(packetTest == deserializedPacket);

     CHECK(deserializedPacket.header_.payloadLength_ == packetTest.header_.payloadLength_);
     CHECK(deserializedPacket.header_.sequenceNumber_ == packetTest.header_.sequenceNumber_);
     CHECK(deserializedPacket.header_.packetType_ == packetTest.header_.packetType_);

     CHECK(deserializedPacket.payload_.size() == packetTest.payload_.size());
     for (size_t i = 0; i < deserializedPacket.payload_.size(); i++) {
         deserializedPacket.payload_[i] == packetTest.payload_[i];
     }
}


TEST_CASE("reinterpret cast playground")
{
    std::byte b{0xFF};
    std::byte* bp = &b;
    std::uint8_t casted = *reinterpret_cast<uint8_t*>(bp);
    std::println("reinterpret_cast={}", casted);

    /**
     * example showing wrong casting, this will not produce the wanted output
     * because of aligning problem
     */
    std::vector<std::byte> byteVec{b, b};
    std::uint16_t casted_16 = *reinterpret_cast<uint16_t*>(byteVec.data());
    std::println("reinterpret_cast uint16_t={}", casted_16);
}
