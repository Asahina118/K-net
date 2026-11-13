#include <cstdint>
#include <string>
#include <array>
#include <format>
#include <string_view>
#include <cstdlib>
#include <print>
#include <stdexcept>
#include <vector>

#include <sys/socket.h>     // socket(), bind(), SOCK_DGRAM, AF_INET
#include <netinet/in.h>     // sockaddr_in, htons()
#include <arpa/inet.h>      // inet_pton()
#include <unistd.h>         // close()

#include "include/knet/endpoint.h"
#include "include/knet/udp/udp_socket.h"

constexpr uint16_t PORT = 9000;
constexpr std::string LOCAL_IP = "127.0.0.1";
constexpr std::size_t BUFFER_SIZE = 1024;

void run()
{
    std::println("running the socket binding phase");
    // domain, type, let the OS to select the default combination of the former two
    // AF_INET = ipv4
    int socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketFd == -1) {
        throw std::runtime_error("could not create socketFd with (AF_INET, SOCK_DGRAM, IPPROTO_UDP)");
    }

    sockaddr_in address{
        .sin_family = AF_INET,
        .sin_port = htons(PORT), 
    };
    inet_pton(AF_INET, LOCAL_IP.data(), &address.sin_addr);

    if (bind(socketFd, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == -1) {
        close(socketFd);
        throw std::runtime_error("could not bind to the socketFd=" + std::to_string(socketFd));
    }
    std::println("successfully binded to address={} port={}", LOCAL_IP, PORT);

    std::array<char, BUFFER_SIZE> buffer{};

    std::array<char, INET_ADDRSTRLEN> clientIp{};

    std::println("entering the event loop for the socketFd={}", socketFd);
    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        /*
            this is a blocking function:
            notice that clientAddrLen will not be written in this case becasue we are making a ipv4 socket,
            but later when we get to the generic ipv4/ipv6 socket handling, the clientAddrLen could be modified to
            represent the actual length of the struct as it could vary between the format of ipv4 and ipv6
        */
        ssize_t readByteSize = recvfrom(socketFd, buffer.data(), buffer.size(), 0,
                reinterpret_cast<sockaddr*>(&clientAddr), 
                &clientAddrLen);
        if (readByteSize == -1) {
            throw std::runtime_error(std::format("recvfrom failed for socketFd={}", socketFd));
        } 
        
        std::string response;
        uint16_t clientPort = ntohs(clientAddr.sin_port);
        if (readByteSize == 0 || buffer[0] == '\n') {
            std::println("no data received from the socketFd={}", socketFd);
            response = "ay nothing received bro do something please....\n";
        } else {
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp.data(), clientIp.size());
            std::println("received data: byte={} from ip={}:{}", readByteSize, clientIp.data(), clientPort);
            std::println("data: \n{}", std::string_view(buffer.data(), readByteSize));
            response = std::string_view(buffer.data(), readByteSize);
            response = "[server response] " + response;
        }

        std::println("ready to send back to the client...");

        ssize_t sentByteSize = sendto(socketFd, response.data(), response.size(), 0, reinterpret_cast<const sockaddr*>(&clientAddr), clientAddrLen);
        if (sentByteSize == -1) {
            close(socketFd);
            throw std::runtime_error(std::format("cannot send type to the clientIp={}:{}", clientIp.data(), clientPort));
        }

    }

    close(socketFd);
}

void run_test_classes()
{
    using namespace knet;
    Endpoint myEndpoint = Endpoint::from(LOCAL_IP, PORT).value();
    udp::UDPSocket socket = udp::UDPSocket::bind(myEndpoint).value();

    std::array<std::byte, BUFFER_SIZE> buffer;
    std::println("listening at {}:{}", LOCAL_IP, PORT);
    while (true) {
        std::expected<udp::ReceivedResult, udp::Error> receiveResult = socket.receive_from(buffer);
        if (!receiveResult) {
            std::println("errors in receiving data");
            continue;
        }

        size_t dataSize = receiveResult->dataSize_;
        std::string clientIp = receiveResult->sourceEndpoint_.get_ip_info();
        if (dataSize == 0) {
            continue;
        }

        std::print("[UDP] received {} bytes from ip={}, message=", dataSize, clientIp);
        std::vector<std::byte> echoBuffer{};
        echoBuffer.resize(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            const std::byte receivedByte = buffer[i];
            echoBuffer[i] = receivedByte;
            std::print("{}", std::to_integer<char>(receivedByte));
        }

        std::expected<size_t, udp::Error> sentResult = socket.send_to(echoBuffer, receiveResult->sourceEndpoint_);
        if (!sentResult) {
            std::println("[UDP] echo failed to ip={}", clientIp);
            continue;
        }
        std::println("[UDP] sent echo to ip={}", clientIp);
    }

}
