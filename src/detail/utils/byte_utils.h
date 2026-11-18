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
        for (size_t i = 0; i < n / 2; i++) {
            std::swap((*bytes)[i], (*bytes)[n - i - 1]);
        }
    }

    template<typename T>
    inline T byte_streams_to_data(Endian endian, std::vector<std::byte> bytes)
    {
        size_t dataLen = sizeof(T);

        std::print("[byte_stream_to_data] byteSize={} parsing bytes: (", bytes.size());
        for (std::byte byte : bytes) {
            std::print("{:02X}, ", static_cast<unsigned int>(byte));
        }
        std::println(")");

        T value;
        if (endian == Endian::Little) {
            invert(&bytes);
            // return *reinterpret_cast<T*>(bytes.data());
            memcpy(&value, bytes.data(), dataLen);

            std::print("[byte_stream_to_data] inverted bytes: ");
            for (std::byte byte : bytes) {
                std::print("{:02X}, ", static_cast<unsigned int>(byte));
            }
            std::println(")");
            std::println("[byte_stream_to_data] parsed value={}", value);

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
        int n;
        int sign;

        std::println("[to_bytes] received value byteSize={}", byteSize);

        if (endian == Endian::Little) {
            sign = 1;
            n = startIdx;
        } else {
            sign = -1;
            n = byteSize + startIdx - 1;
        }

        std::byte mask = static_cast<std::byte>(0xFF);

        for (size_t i = 0; i < byteSize; i++, value >>= 8) {
            std::println("[to_bytes] making byte[{}] = {:02X}", n+ sign*i, static_cast<unsigned int>(value & 0xFF));
            (*bytes)[n + sign*i] = static_cast<std::byte>(value & 0xFF);
        }
    }
}
