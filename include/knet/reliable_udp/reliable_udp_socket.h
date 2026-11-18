#pragma once

#include "include/knet/udp/udp_socket.h"

namespace knet::reliable_udp
{
    enum class Error 
    {
        socket_creation_failed,
        socket_binding_failed,
        send_to_failed,
        data_not_fully_sent,
        receive_from_failed,
    };

    struct ReceivedResult
    {
        Endpoint sourceEndpoint_;
        size_t dataSize_;
    };

    // TODO : implement
    class ReliableUDPSocket
    {
    public:
        static std::expected<ReliableUDPSocket, reliable_udp::Error> bind(Endpoint localEndpoint);

        ReliableUDPSocket(const ReliableUDPSocket& other) = delete;
        ReliableUDPSocket& operator=(const ReliableUDPSocket& other) = delete;

        ReliableUDPSocket(ReliableUDPSocket&& other) noexcept = default;
        ReliableUDPSocket& operator=(ReliableUDPSocket&& other) noexcept = default;

        std::expected<size_t, reliable_udp::Error> send_to(
                std::span<const std::byte> data,
                const Endpoint& destination
                );

        std::expected<ReceivedResult, udp::Error> receive_from(std::span<std::byte> buffer);

    private:
        explicit ReliableUDPSocket(udp::UDPSocket udpSocket);
        udp::UDPSocket udpSocket;
    };
}
