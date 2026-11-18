#include <cstddef>
#include <print>

#include "tests/extern/doctest.h"
#include "include/knet/reliable_udp/packet.h"
#include "src/detail/utils/byte_utils.h"

void test_serialize_and_deserialize(knet::reliable_udp::packet::Packet& packet)
{
    using namespace knet::reliable_udp::packet;

    auto serializedResult = serializer::serialize(packet);
    REQUIRE(serializedResult.has_value());

    auto deserializedResult = serializer::deserialize(*serializedResult);
    REQUIRE(deserializedResult.has_value());

    // CHECK_EQ(*deserializedResult, packet);
    Packet deserializedPacket = *deserializedResult;

    auto deserializedPacketType = static_cast<std::uint8_t>(deserializedPacket.header_.packetType_);
    auto deserializedPayloadLength = deserializedPacket.header_.payloadLength_;
    auto deserializedSequenceNumber = deserializedPacket.header_.sequenceNumber_;
    auto deserializedPacketPayload = deserializedPacket.payload_;

    std::println("[packet comparison] checking deserialized packet against original packet");
    std::println("packet type: {} , {}", deserializedPacketType, static_cast<std::uint8_t>(packet.header_.packetType_));
    CHECK_EQ(deserializedPacketType, static_cast<std::uint8_t>(packet.header_.packetType_));

    std::println("payload length: {} , {}", deserializedPayloadLength, packet.header_.payloadLength_);
    CHECK_EQ(deserializedPayloadLength, packet.header_.payloadLength_);

    std::println("sequence number: {} , {}", deserializedSequenceNumber, packet.header_.sequenceNumber_);
    CHECK_EQ(deserializedSequenceNumber, packet.header_.sequenceNumber_);

    std::vector<std::byte> originalPayload(packet.payload_.begin(), packet.payload_.begin() + packet.header_.payloadLength_);
    CHECK_EQ(deserializedPacketPayload, originalPayload);
}

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

    test_serialize_and_deserialize(packetTest);

    packetTest = Packet {
        .header_ = PacketHeader {
            .sequenceNumber_ = 6888,
            .payloadLength_ = 0,
            .packetType_ = PacketType::ACK
        },
        .payload_ = std::vector<std::byte>(944, std::byte{0xFF})
    };

    test_serialize_and_deserialize(packetTest);

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
