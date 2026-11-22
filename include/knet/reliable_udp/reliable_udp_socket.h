#pragma once

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

        std::uint32_t nextSeqNumIn_;
        std::uint32_t nextSeqNumOut_;
        State state_;
    };


    /* design note:
     *
     * currently only supports predefined endpoint. No server logic yet
     * the socket that initializes the handshake keeps being on the active side, no usage case for both-side initialization yet
     * otherwise would need a random id to decide the handshake initialzer on hello package conflicts
     */
}
