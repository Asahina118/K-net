#include "include/knet/reliable_udp/reliable_udp_socket.h"
#include "include/knet/reliable_udp/packet.h"
#include "include/knet/udp/udp_socket.h"
#include <expected>

namespace knet::reliable_udp
{
    ReliableUDPSocket::ReliableUDPSocket(udp::UDPSocket udpSocket)
        : udpSocket_(std::move(udpSocket))
    {}

    std::expected<ReliableUDPSocket, reliable_udp::Error> 
        ReliableUDPSocket::bind(Endpoint localEndpoint)
    {
        if (!udp::UDPSocket::bind(localEndpoint)) {
            return std::unexpected(Error::socket_creation_failed);
        }

        return ReliableUDPSocket(*udp::UDPSocket::bind(localEndpoint));
    }

    std::expected<size_t, reliable_udp::Error> send_to(
            std::span<const std::byte> data,
            const Endpoint& destination
            )
    {
        return {};
    }

    /*
     * this function gets called after the registration of source endpoint is completed
     * so we need a init function for that
     * then also add a reset function for resetting this socket state
     */
    std::expected<ReceivedResult, reliable_udp::Error> 
        ReliableUDPSocket::receive(std::span<std::byte> buffer)
    {
        using namespace packet;

        // -1 is handled
        std::expected<udp::ReceivedResult, udp::Error> udpResult = udpSocket_.receive_from(buffer);

        if (!udpResult || udpResult->dataSize_ < PACKET_HEADER_SIZE_NO_PADDING) {
            // 
        }

        std::expected<Packet, reliable_udp::Error> packet = serializer::deserialize_header(buffer);
        if (!packet) {
            // TODO: log error, retry
        }

        std::uint32_t recvSeqNum = packet->header_.seqNum_;

        if (recvSeqNum < nextSeqNum_) [[unlikely]] {
            // TODO : ask for resend of previous
        } else if (recvSeqNum > nextSeqNum_) [[unlikely]] {
            // TODO : store into buffer first
        }
        
        // TODO : after receiving successfully, send a packet with acknowledgement with the next expected sequence number
        return std::unexpected(Error::socket_creation_failed);
    }

    Endpoint ReliableUDPSocket::get_local_endpoint() const
    {
        return udpSocket_.get_local_endpoint();
    }

    Endpoint ReliableUDPSocket::get_source_endpoint() const
    {
        return sourceEndpoint_;
    }
}
