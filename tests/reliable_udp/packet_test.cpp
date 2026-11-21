#include <cstddef>
#include <expected>
#include <limits>
#include <print>
#include <stdexcept>

#include "include/knet/reliable_udp/error.h"
#include "tests/extern/doctest.h"
#include "include/knet/reliable_udp/packet.h"
#include "src/detail/utils/byte_utils.h"

std::expected<void, knet::reliable_udp::Error> test_serialize_and_deserialize(knet::reliable_udp::packet::Packet& packet)
{
    using namespace knet::reliable_udp::packet;

    auto serializedResult = serializer::serialize_header(packet);
    if (!serializedResult) return std::unexpected(serializedResult.error());

    auto deserializedResult = serializer::deserialize_header(*serializedResult);
    if (!deserializedResult) return std::unexpected(deserializedResult.error());

    Packet deserializedPacket = *deserializedResult;

    auto deserializedPacketType = static_cast<std::uint8_t>(deserializedPacket.header_.packetType_);
    auto deserializedPayloadLength = deserializedPacket.header_.payloadLength_;
    auto deserializedSequenceNumber = deserializedPacket.header_.seqNum_;
    auto deserializedPacketPayload = deserializedPacket.payload_;

    std::println("[packet comparison] checking deserialized packet against original packet");
    std::println("packet type: {} , {}", deserializedPacketType, static_cast<std::uint8_t>(packet.header_.packetType_));
    CHECK_EQ(deserializedPacketType, static_cast<std::uint8_t>(packet.header_.packetType_));

    std::println("payload length: {} , {}", deserializedPayloadLength, packet.header_.payloadLength_);
    CHECK_EQ(deserializedPayloadLength, packet.header_.payloadLength_);

    std::println("sequence number: {} , {}", deserializedSequenceNumber, packet.header_.seqNum_);
    CHECK_EQ(deserializedSequenceNumber, packet.header_.seqNum_);

    std::vector<std::byte> originalPayload(packet.payload_.begin(), packet.payload_.begin() + packet.header_.payloadLength_);
    CHECK_EQ(deserializedPacketPayload, originalPayload);

    return {};
}

TEST_CASE("packet serialization test")
{
    using namespace knet::reliable_udp::packet;
    using namespace knet::detail::byte_utils;

    Packet packetHappyFlow = Packet {
        .header_ = PacketHeader {
            .seqNum_ = 1,
            .payloadLength_ = 5,
            .packetType_ = PacketType::DATA
        },
        .payload_ = std::vector<std::byte>(5, std::byte{0xFF})
    };
    CHECK(test_serialize_and_deserialize(packetHappyFlow).has_value());

    auto packetZeroPayloadLen = Packet {
        .header_ = PacketHeader {
            .seqNum_ = 6888,
            .payloadLength_ = 0,
            .packetType_ = PacketType::ACK
        },
        .payload_ = std::vector<std::byte>(944, std::byte{0xFF})
    };
    CHECK(test_serialize_and_deserialize(packetZeroPayloadLen).has_value());

    auto packetHeaderLenTooLong = Packet {
        .header_ = PacketHeader {
            .seqNum_ = std::numeric_limits<decltype(PacketHeader::seqNum_)>::max(),
            .payloadLength_ = std::numeric_limits<decltype(PacketHeader::payloadLength_)>::max(),
            .packetType_ = PacketType::FIN
        },
        .payload_ = std::vector<std::byte>(944, std::byte{0xFF})
    };
    CHECK(!test_serialize_and_deserialize(packetHeaderLenTooLong).has_value());

    auto packetMaxValue = Packet {
        .header_ = PacketHeader {
            .seqNum_ = MAX_SEQUENCE_NUMBER,
            .payloadLength_ = MAX_SEQUENCE_PAYLOAD_LENGTH,
            .packetType_ = PacketType::FIN
        },
        .payload_ = std::vector<std::byte>(MAX_SEQUENCE_PAYLOAD_LENGTH, std::byte{0xFF})
    };
    CHECK(test_serialize_and_deserialize(packetMaxValue).has_value());
}

std::expected<knet::reliable_udp::packet::Packet, knet::reliable_udp::Error>
receive_from(std::span<const std::byte> buffer, ssize_t recv_from_return_value)
{
    using namespace knet::reliable_udp;

    if (recv_from_return_value == -1) {
        return std::unexpected(Error::socket_receive_failed);
    }

    if (recv_from_return_value < packet::PACKET_HEADER_SIZE_NO_PADDING) {
        return std::unexpected(Error::socket_received_less_byte_than_header_size);
    }

    std::span<const std::byte> toBeDeserialized(buffer.begin(), buffer.begin() + recv_from_return_value);

    return packet::serializer::deserialize_header(toBeDeserialized);
}

TEST_CASE("receive packet")
{
    using namespace knet::reliable_udp::packet;
    using namespace knet::reliable_udp;

    std::array<std::byte, 512> recvBuffer;
    recvBuffer.fill(std::byte(0xFF));
    ssize_t mock_recv_from_return_value = -1;

    // note : std::expected can be compared to std::unexpected becasue the bool operator==(const std::unexpected&) is well defined in the implementation
    CHECK_EQ(
            receive_from(recvBuffer, mock_recv_from_return_value), 
            std::unexpected(knet::reliable_udp::Error::socket_receive_failed)
            );

    mock_recv_from_return_value = 5;
    CHECK_EQ(
            receive_from(recvBuffer, mock_recv_from_return_value), 
            std::unexpected(knet::reliable_udp::Error::socket_received_less_byte_than_header_size)
            );

    mock_recv_from_return_value = 7;
    recvBuffer[5] = std::byte(0x00);
    recvBuffer[6] = std::byte(0x00);

    auto result = receive_from(recvBuffer, mock_recv_from_return_value);

    CHECK_EQ(
            result.error(),
            Error::deserializer_unknown_packet_type
            );
    
    recvBuffer[0] = std::byte(0x00);
    result = receive_from(recvBuffer, mock_recv_from_return_value);

    REQUIRE(result.has_value());

    CHECK_EQ(
            result->payload_.size(),
            0
            );
}
