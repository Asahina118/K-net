#include "include/knet/reliable_udp/reliable_udp_socket.h"
#include "include/knet/reliable_udp/error.h"
#include "include/knet/reliable_udp/packet.h"
#include "include/knet/udp/udp_socket.h"
#include <expected>

namespace knet::reliable_udp
{
    ReliableUDPSocket::ReliableUDPSocket(udp::UDPSocket udpSocket, Endpoint peerEndpoint) :
        udpSocket_(std::move(udpSocket)),
        peerEndpoint_(peerEndpoint),
        state_(State::init),
        nextSeqNumIn_(0),
        nextSeqNumOut_(0)
    {}

    std::expected<ReliableUDPSocket, reliable_udp::Error> 
        ReliableUDPSocket::establish(Endpoint localEndpoint, Endpoint peerEndpoint)
    {
        auto bindResult = udp::UDPSocket::bind(localEndpoint);
        if (!bindResult) {
            return std::unexpected(Error::socket_creation_failed);
        }
        // TODO: implement active connection
        return ReliableUDPSocket(std::move(*bindResult), peerEndpoint);
    }

    std::expected<ReliableUDPSocket, reliable_udp::Error> 
        ReliableUDPSocket::listen(Endpoint localEndpoint, Endpoint peerEndpoint)
    {
        auto bindResult = udp::UDPSocket::bind(localEndpoint);
        if (bindResult) {
            return std::unexpected(Error::socket_creation_failed);
        }
        // TODO: implement passive connection
        return ReliableUDPSocket(std::move(*bindResult), peerEndpoint);
    }


    std::expected<size_t, reliable_udp::Error> send(
            std::span<const std::byte> data,
            const Endpoint& destination
            )
    {
        return {};
    }

    // std::expected

    /*
     * this function gets called after the registration of source endpoint is completed
     * so we need a init function for that
     * then also add a reset function for resetting this socket state
     */
    std::expected<ReceivedResult, reliable_udp::Error> 
        ReliableUDPSocket::receive(std::span<std::byte> buffer)
    {
        using namespace packet;

        if (state_ != State::established) [[unlikely]] {
            return state_ == State::stopped ? 
                std::unexpected(Error::socket_recv_connection_stopped)
                : std::unexpected(Error::socket_unestablished_connection);
        }

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

        if (recvSeqNum < nextSeqNumIn_) [[unlikely]] {
            // TODO : 
            // we never request a package of seqNum_ = n before sending ACK(n)
            // this only happens when the ACK(n) is lost, and the sender redelivers the old seqNum
            // then in this case we ignore the package, and resend the ACK(n)
        } else if (recvSeqNum > nextSeqNumIn_) [[unlikely]] {
            // TODO : store into buffer first
        }
        
        // TODO : after receiving successfully, send a packet with acknowledgement with the next expected sequence number
        return std::unexpected(Error::socket_creation_failed);
    }

    Endpoint ReliableUDPSocket::get_local_endpoint() const
    {
        return udpSocket_.get_local_endpoint();
    }

    Endpoint ReliableUDPSocket::get_peer_endpoint() const
    {
        return peerEndpoint_;
    }
}
