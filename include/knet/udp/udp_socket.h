#pragma once

#include <cstddef> // std::byte (since C++17)
#include <expected>
#include <span>

#include "src/detail/socket_handle.h"
#include "include/knet/endpoint.h"

namespace knet::udp
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

    class UDPSocket
    {
    public:
        static std::expected<UDPSocket, udp::Error> bind(Endpoint localEndpoint);

        UDPSocket(const UDPSocket& other) = delete;
        UDPSocket& operator=(const UDPSocket& other) = delete;

        UDPSocket(UDPSocket&& other) noexcept = default;
        UDPSocket& operator=(UDPSocket&& other) noexcept = default;

        std::expected<size_t, udp::Error> send_to(
                std::span<const std::byte> data,
                const Endpoint& destination
                );

        std::expected<ReceivedResult, udp::Error> receive_from(std::span<std::byte> buffer);

    private:
        explicit UDPSocket(Endpoint localEndpoint, detail::SocketHandle socket);

        detail::SocketHandle socket_;
        Endpoint localEndpoint_;
    };
}
