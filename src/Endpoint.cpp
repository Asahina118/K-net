#include "Endpoint.h"

#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <string>
#include <format>
#include <arpa/inet.h>      // inet_pton()

namespace knet 
{

    // todo: add support to ipv6 in the future
    Endpoint::Endpoint(const std::string& ip, uint16_t port)
        : address_{}
    {
        address_.sin_family = AF_INET;
        address_.sin_port = htons(port);

        int success = inet_pton(AF_INET, ip.c_str(), &address_.sin_addr);
        if (success <= 0) {
            throw std::invalid_argument(std::format("failed to construct the endpoint for the ipv4_address={}:{}", ip, port));
        };
    }

    Endpoint::Endpoint(sockaddr_in address)
        : address_(address)
    {}

    static std::expected<Endpoint, EndpointError>
        from(const std::string& ip, uint16_t port)
    {
        sockaddr_in address = sockaddr_in {
            .sin_family = AF_INET,
            .sin_port = htons(port)
        };

        int success = inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
        if (success == -1) {
            return std::unexpected(EndpointError::SystemError);
        } else if (success == 0) {
            return std::unexpected(EndpointError::InvalidAddress);
        }

        return Endpoint(address);
    }

    const sockaddr_in& Endpoint::native() const
    {
        return address_;
    }

}

