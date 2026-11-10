#include "udp_socket.h"
#include "src/Endpoint.h"

#include <cstddef>
#include <expected>
#include <sys/socket.h>
#include <utility>

namespace knet::udp 
{
    UDPSocket::UDPSocket(Endpoint localEndpoint, detail::SocketHandle socket) 
        : localEndpoint_(std::move(localEndpoint)), socket_(std::move(socket))
    {}

    ssize_t UDPSocket::send_to(std::span<const std::byte> data, const Endpoint& destination)
    {
        int sourceFd = socket_.get_fd();
        const sockaddr_in& destinationAddress = destination.native();
        socklen_t destinationLen = sizeof(destination);
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
