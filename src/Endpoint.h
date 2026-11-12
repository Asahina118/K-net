#pragma once

#include <expected>
#include <netinet/in.h>
#include <string>

namespace knet
{
    enum class EndpointError 
    {
        InvalidAddress,
        SystemError
    };

    class Endpoint
    {
    public:
        explicit Endpoint(sockaddr_in address);

        static std::expected<Endpoint, EndpointError>
            from(const std::string& ip, uint16_t port);

        const sockaddr_in& native() const;

        std::string get_ip_info() const;

    private:
        explicit Endpoint(sockaddr_in address, std::string ip, uint16_t port);

        sockaddr_in address_;
        std::string ip_;
        uint16_t port_;
    };
};
