#include "include/knet/udp/udp_socket.h"
#include "include/knet/endpoint.h"

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

    std::expected<size_t, udp::Error>
        UDPSocket::send_to(std::span<const std::byte> data, const Endpoint& destination)
    {
        int sourceFd = socket_.get_fd();
        const sockaddr_in& destinationAddress = destination.native();
        socklen_t destinationLen = sizeof(destinationAddress);
        ssize_t numSentBytes = sendto(
                sourceFd,
                data.data(),
                data.size(),
                0,
                reinterpret_cast<const sockaddr*>(&destinationAddress),
                destinationLen
        );
        if (numSentBytes == -1) {
            return std::unexpected(udp::Error::send_to_failed);
        } else if (numSentBytes != data.size_bytes()) {
            return std::unexpected(udp::Error::data_not_fully_sent);
        }

        return static_cast<size_t>(numSentBytes);
    }

    /*
     * todo : i dont like the buffer being a out parameter, wrap it inside expected next
     */
    std::expected<ReceivedResult, udp::Error>
        UDPSocket::receive_from(std::span<std::byte> buffer)
    {
        sockaddr_in sourceAddress {};
        socklen_t sourceAddressLen = sizeof(sourceAddress);
        ssize_t numReceivedBytes = recvfrom(
                socket_.get_fd(), 
                buffer.data(), 
                buffer.size(), 
                0, 
                reinterpret_cast<sockaddr*>(&sourceAddress), 
                &sourceAddressLen
                );

        if (numReceivedBytes == -1) {
            return std::unexpected(udp::Error::receive_from_failed);
        }
        Endpoint sourceEndpoint(sourceAddress);

        ReceivedResult receiveResult = ReceivedResult {
            .sourceEndpoint_ = sourceEndpoint,
            .dataSize_ = static_cast<size_t>(numReceivedBytes)
        };

        return receiveResult;
    }

}
