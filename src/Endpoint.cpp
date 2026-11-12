#include "endpoint.h"

#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <string>
#include <format>
#include <arpa/inet.h>      // inet_pton()

namespace knet 
{
    // constructed from correctly populated address
    Endpoint::Endpoint(sockaddr_in address)
        : address_(address), port_(ntohs(address.sin_port))
    {
        ip_ = std::string{};
        ip_.resize(INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &address.sin_addr, ip_.data(), ip_.size());
    }

    Endpoint::Endpoint(sockaddr_in address, std::string ip, uint16_t port)
        : address_(address), ip_(std::move(ip)), port_(port)
    {}

    std::expected<Endpoint, EndpointError>
        Endpoint::from(const std::string& ip, uint16_t port)
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

        return Endpoint(address, ip, port);
    }

    const sockaddr_in& Endpoint::native() const
    {
        return address_;
    }

    std::string Endpoint::get_ip_info() const
    {
        return std::format("{}:{}", ip_, port_);
    };

}

