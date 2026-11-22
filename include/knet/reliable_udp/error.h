#pragma once

namespace knet::reliable_udp
{
    enum class Error
    {
        serializer_header_len_bigger_than_actual_payload_len,

        deserializer_not_enough_byte_to_parse_header,
        deserializer_payload_len_does_not_match_header_parsed_len,
        deserializer_unknown_packet_type,

        socket_creation_failed,
        socket_receive_failed,
        socket_received_less_byte_than_header_size,
        socket_unestablished_connection,
        socket_recv_connection_stopped
    };
}
