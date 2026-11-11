#pragma once

#include <cstddef> // std::byte (since C++17)
#include <expected>
#include <span>

#include "src/detail/socket_handle.h"
#include "src/Endpoint.h"

namespace knet::udp
{
    enum class Error 
    {
        socket_creation_failed,
        socket_binding_failed
    };

    class UDPSocket
    {
    public:
        static std::expected<UDPSocket, udp::Error> bind(Endpoint localEndpoint);

        UDPSocket(const UDPSocket& other) = delete;
        UDPSocket& operator=(const UDPSocket& other) = delete;

        UDPSocket(UDPSocket&& other) noexcept = default;
        UDPSocket& operator=(UDPSocket&& other) noexcept = default;

        ssize_t send_to(
                std::span<const std::byte> data,
                const Endpoint& destination
        );

    private:
        explicit UDPSocket(Endpoint localEndpoint, detail::SocketHandle socket);

        detail::SocketHandle socket_;
        Endpoint localEndpoint_;
    };
}
