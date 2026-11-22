#pragma once

#include "include/knet/reliable_udp/packet.h"
#include "include/knet/udp/udp_socket.h"
#include "include/knet/reliable_udp/error.h"

#include <cstdint>

namespace knet::reliable_udp
{
    struct ReceivedResult
    {
        Endpoint sourceEndpoint_;
        size_t dataSize_;
    };

    // TODO : implement
    class ReliableUDPSocket
    {
    public:

        // captures all handshake logics, only returns an established socket
        static std::expected<ReliableUDPSocket, reliable_udp::Error> 
            establish(Endpoint localEndpoint, Endpoint peerEndpoint);

        static std::expected<ReliableUDPSocket, reliable_udp::Error> 
            listen(Endpoint localEndpoint, Endpoint peerEndpoint);

        ReliableUDPSocket(const ReliableUDPSocket& other) = delete;
        ReliableUDPSocket& operator=(const ReliableUDPSocket& other) = delete;

        ReliableUDPSocket(ReliableUDPSocket&& other) noexcept = default;
        ReliableUDPSocket& operator=(ReliableUDPSocket&& other) noexcept = default;


        // no handshake logic, only deals with two states: 
        // stopped / established
        std::expected<size_t, reliable_udp::Error> 
            send(std::span<const std::byte> data);

        std::expected<ReceivedResult, reliable_udp::Error> 
            receive(std::span<std::byte> buffer);

        Endpoint get_local_endpoint() const;
        Endpoint get_peer_endpoint() const;

    private:
        // passive socket: waits for handshake init
        enum class State : uint8_t
        {
            init = 0,
            listening,
            awaiting_hello_ack,
            awaiting_ack,
            established,
            stopped
        };

        explicit ReliableUDPSocket(udp::UDPSocket udpSocket, Endpoint peerEndpoint);

        udp::UDPSocket udpSocket_;
        Endpoint peerEndpoint_;

        static constexpr std::size_t MAX_BUFFER_SIZE = packet::MAX_SEQUENCE_PAYLOAD_LENGTH;
        std::array<std::byte, MAX_BUFFER_SIZE> buffer_;

        std::uint32_t nextSeqNumIn_;
        std::uint32_t nextSeqNumOut_;
        State state_;

        std::expected<void, reliable_udp::Error> handle_handshake_state();
        
        std::expected<packet::Packet, reliable_udp::Error>
            parse_packet_header(udp::ReceivedResult);

        std::expected<void, reliable_udp::Error> handshake_listening();

        [[gnu::always_inline]] inline std::expected<size_t, udp::Error> udp_send_to_peer(std::span<const std::byte> payload)
        {
            return udpSocket_.send_to(payload, peerEndpoint_);
        };

    };


    /* design note:
     *
     * currently only supports predefined endpoint. No server logic yet
     * the socket that initializes the handshake keeps being on the active side, no usage case for both-side initialization yet
     * otherwise would need a random id to decide the handshake initialzer on hello package conflicts
     */
}
