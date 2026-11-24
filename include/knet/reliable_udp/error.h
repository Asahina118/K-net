#pragma once
#include <string>

#define ERRORS \
    X(serializer_header_len_bigger_than_actual_payload_len) \
    \
    X(deserializer_unknown_packet_type) \
    X(deserializer_not_enough_byte_to_parse_header) \
    X(deserializer_payload_len_does_not_match_header_parsed_len) \
    \
    X(socket_creation_failed) \
    X(socket_receive_failed) \
    X(socket_received_less_byte_than_header_size) \
    X(socket_unestablished_connection) \
    X(socket_recv_connection_stopped) \
    \
    X(handshake_listening_failed) \
    X(handshake_expected_peer_endpoint) \
    X(handshake_expected_SYN) \
    X(handshake_send_SYN_ACK_failed)


namespace knet::reliable_udp
{
    enum class Error
    {
        #define X(err) err,
            ERRORS
        #undef X
    };

    inline std::string_view get_msg(Error error)
    {
        switch (error) {
            #define X(error) \
                case Error::error: \
                    return #error;
                ERRORS
            #undef X
        }
        return "unknown error";
    }
}
