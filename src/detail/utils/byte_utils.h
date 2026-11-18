#pragma once

#include <cstdint>
#include <cstddef>
#include <print>
#include <vector>
#include <cstring>

namespace knet::detail::byte_utils
{
    enum class Endian : std::int8_t
    {
        Little = 0,
        Big = 1
    };

    inline void invert(std::vector<std::byte>* bytes)
    {
        size_t n = bytes->size();
        std::println("byte size is {}", n);
        for (size_t i = 0; i < n / 2; i++) {
            std::println("inverting {} and {}", i, n - i - 1);
            std::swap((*bytes)[i], (*bytes)[n - i - 1]);
        }
    }

    template<typename T>
    inline T byte_streams_to_data(Endian endian, std::vector<std::byte> bytes)
    {
        size_t dataLen = sizeof(T);
        std::println("[byte_stream_to_data] bytes.size()={}, dataLen={}", bytes.size(), dataLen);
        for (std::byte byte : bytes) {
            std::print("{:02X}, ", static_cast<unsigned int>(byte));
        }

        T value;
        if (endian == Endian::Little) {
            invert(&bytes);
            // return *reinterpret_cast<T*>(bytes.data());
            memcpy(&value, bytes.data(), dataLen);
            return value;
        } else {
            memcpy(&value, bytes.data(), dataLen);
            return value;
        }
    }

    template<typename T>
    inline void
    to_bytes(T value, Endian endian, std::vector<std::byte>* bytes, size_t startIdx)
    {
        const size_t byteSize = sizeof(T);
        std::println("the byteSize={}", sizeof(T));
        int n;
        int sign;

        if (endian == Endian::Little) {
            sign = 1;
            n = startIdx;
        } else {
            sign = -1;
            n = byteSize + startIdx - 1;
        }

        constexpr std::byte mask = static_cast<std::byte>(0xFF);
        auto byteValue = static_cast<std::byte>(value);
        for (size_t i = 0; i < byteSize; i++, byteValue >>= 8) {
            std::println("making byte[{}] = {:02X}", n+ sign*i, static_cast<unsigned int>(byteValue & mask));
            (*bytes)[n + sign*i] = byteValue & mask;
        }
    }
}
