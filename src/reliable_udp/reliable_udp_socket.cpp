#include "include/knet/reliable_udp/reliable_udp_socket.h"
#include "include/knet/udp/udp_socket.h"
#include <expected>

namespace knet::reliable_udp
{
    ReliableUDPSocket(udp::UDPSocket udpSocket)
    {

    }

    static std::expected<ReliableUDPSocket, reliable_udp::Error> 
        bind(Endpoint localEndpoint)
    {
        return ReliableUDPSocket();
    }

    ReliableUDPSocket(const ReliableUDPSocket& other) = delete;
    ReliableUDPSocket& operator=(const ReliableUDPSocket& other) = delete;

    ReliableUDPSocket(ReliableUDPSocket&& other) noexcept = default;
    ReliableUDPSocket& operator=(ReliableUDPSocket&& other) noexcept = default;

    std::expected<size_t, reliable_udp::Error> send_to(
            std::span<const std::byte> data,
            const Endpoint& destination
            );

    std::expected<ReceivedResult, udp::Error> receive_from(std::span<std::byte> buffer);
}
