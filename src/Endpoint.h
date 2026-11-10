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
        explicit Endpoint(const std::string& ip, uint16_t port);

        explicit Endpoint(sockaddr_in address);

        static std::expected<Endpoint, EndpointError>
            from(const std::string& ip, uint16_t port);

        const sockaddr_in& native() const;

    private:
        sockaddr_in address_;
    };
};
