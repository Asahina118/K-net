#pragma once

#include <cstddef> // std::byte (since C++17)
#include <span>

#include "src/detail/socket_handle.h"
#include "src/Endpoint.h"

namespace knet::udp
{
    class UDPSocket
    {
    public:
        explicit UDPSocket(Endpoint localEndpoint, detail::SocketHandle socket);

        UDPSocket(const UDPSocket& other) = delete;
        UDPSocket& operator=(const UDPSocket& other) = delete;

        UDPSocket(UDPSocket&& other) noexcept = default;
        UDPSocket& operator=(UDPSocket&& other) noexcept = default;

        ssize_t send_to(
                std::span<const std::byte> data,
                const Endpoint& destination
        );

    private:
        detail::SocketHandle socket_;

        // todo : maybe this is not useful as the socket interface assigns you a port number?
        Endpoint localEndpoint_;
    };
}
