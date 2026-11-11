#include "udp_socket.h"
#include "src/Endpoint.h"

#include <cstddef>
#include <expected>
#include <netinet/in.h>
#include <sys/socket.h>
#include <utility>

namespace knet::udp 
{
    std::expected<UDPSocket, udp::Error> 
        UDPSocket::bind(Endpoint localEndpoint)
    {
        int socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socketFd == -1) {
            return std::unexpected(udp::Error::socket_creation_failed);
        }
        detail::SocketHandle socket(socketFd);

        const sockaddr_in& localAddress = localEndpoint.native();

        int bindSuccess = ::bind(
                socket.get_fd(), 
                reinterpret_cast<const sockaddr*>(&localAddress), 
                sizeof(localAddress)
                );

        if (bindSuccess == -1) {
            return std::unexpected(udp::Error::socket_binding_failed);
        }

        return UDPSocket(
                std::move(localEndpoint), 
                std::move(socket)
                );
    }

    UDPSocket::UDPSocket(Endpoint localEndpoint, detail::SocketHandle socket) 
        : localEndpoint_(std::move(localEndpoint)), socket_(std::move(socket))
    {}

    ssize_t UDPSocket::send_to(std::span<const std::byte> data, const Endpoint& destination)
    {
        int sourceFd = socket_.get_fd();
        const sockaddr_in& destinationAddress = destination.native();
        socklen_t destinationLen = sizeof(destinationAddress);
        return sendto(
                sourceFd,
                data.data(),
                data.size(),
                0,
                reinterpret_cast<const sockaddr*>(&destinationAddress),
                destinationLen
        );
    }



}
