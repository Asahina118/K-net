#include "include/knet/reliable_udp/packet.h"

#include "src/detail/utils/byte_utils.h"
#include <cstddef>
#include <expected>
#include <limits>

namespace knet::reliable_udp::packet
{
    PacketType parse_packet_type_from_byte(const std::byte byte)
    {
        std::uint8_t unsignedByte = static_cast<std::uint8_t>(byte);
        switch (unsignedByte) {
            case 0 : return PacketType::DATA;
            case 1 : return PacketType::ACK;
            case 2 : return PacketType::SYN;
            case 3 : return PacketType::FIN;
            default : return PacketType::UNKNOWN;
        }
    }

    std::expected<std::vector<std::byte>, reliable_udp::Error>
        serializer::serialize(const Packet& packet)
    {
        using namespace knet::detail;

        if (packet.payload_.size() < packet.header_.payloadLength_) {
            return std::unexpected(reliable_udp::Error::header_len_bigger_than_actual_payload_len);
        }

        const size_t BYTE_SIZE = PACKET_HEADER_SIZE_NO_PADDING + packet.header_.payloadLength_;
        std::vector<std::byte> bytes(BYTE_SIZE);
        size_t startIdx = 0;
        auto endian = byte_utils::Endian::Big;

        byte_utils::to_bytes(static_cast<uint8_t>(packet.header_.packetType_),
                endian,
                &bytes,
                startIdx
                );

        // std::println("[serialize] sequence number={}", packet.header_.sequenceNumber_);
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

        std::print("[serialize] serialized bytes: (");
        for (std::byte byte : bytes) {
            std::print("{:02X}, ", static_cast<std::uint8_t>(byte));
        }
        std::println(")");
        return bytes;
    }

    std::expected<Packet, reliable_udp::Error>
        serializer::deserialize(std::span<const std::byte> payload)
    {
        using namespace knet::detail;

        std::print("[deserialize] received payload: (");
        for (std::byte byte : payload) {
            std::print("{:02X}, ", static_cast<std::uint8_t>(byte));
        }
        std::println(")");

        // to be specific this is from big endian to little endian. Only do inverting when they are XOR
        auto endian = byte_utils::Endian::Little;

        auto sequenceNumberOffsetIt = payload.begin() + PACKET_SEQUENCE_NUMBER_OFFSET;
        std::uint32_t sequenceNumber = byte_streams_to_data<std::uint32_t>(
                endian, 
                std::vector<std::byte>(sequenceNumberOffsetIt, sequenceNumberOffsetIt + sizeof(PacketHeader::sequenceNumber_))
                );

        auto payloadLengthOffsetIt = payload.begin() + PACKET_PAYLOAD_LENGTH_OFFSET;
        std::uint16_t payloadLength = byte_streams_to_data<std::uint16_t>(
                endian,
                std::vector<std::byte>(payloadLengthOffsetIt, payloadLengthOffsetIt + sizeof(PacketHeader::payloadLength_))
                );

        // 1 byte enum: size is very unlikely to change. Safe to hardcode
        PacketType packetType = parse_packet_type_from_byte(payload[PACKET_PACKET_TYPE_OFFSET]);

        std::vector<std::byte> bytes(payloadLength);
        for (size_t i = 0; i < payloadLength; i++) {
            bytes[i] = payload[PACKET_PAYLOAD_OFFSET + i];
        }

        return Packet {
            .header_ = PacketHeader {
                .sequenceNumber_ = sequenceNumber,
                .payloadLength_ = payloadLength,
                .packetType_ = packetType
            },
            .payload_ = bytes
        };
    }
}

/* use case for (reliable udp implementation?): 
    *
    * ReceivedResult result = recvmsg(endpoint);
    * log("received " + result.buffer_ + " raw bytes");
    * Packet packet = deserialize(result->buffer_);
    * if (packet) {
    *   reliable mechanisms: keep track of the sequence number (in where? in the UDPSocket?)
    * }
    */
