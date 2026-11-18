#pragma once

#include <cstdint>
#include <cstddef>
#include <print>
#include <vector>

namespace knet::detail::byte_utils
{
    enum class Endian : std::int8_t
    {
        Little = 0,
        Big = 1
    };

    static inline void invert(std::vector<std::byte>* bytes)
    {
        size_t n = bytes->size();
        // n / 2 might be wrong. Need to check later
        for (size_t i = 0; i < n / 2; i++) {
            bytes[i] = bytes[n - i];
        }
    }

    template<typename T>
    static inline void
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
