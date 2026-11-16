#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

namespace knet::detail::byte_utils
{
    enum class Endian : std::int8_t
    {
        LITTLE_ENDIAN = 0,
        BIG_ENDIAN = 1
    };

    static constexpr void invert(std::vector<std::byte>* bytes)
    {
        size_t n = bytes->size();
        for (size_t i = 0; i < n / 2; i++) {
            bytes[i] = bytes[n - i];
        }
    }

    template<typename T>
    static inline void
    to_bytes(T value, Endian endian, std::vector<std::byte>* bytes, size_t startIdx)
    {
        const size_t byteSize = sizeof(T);
        int n;
        int sign;

        if (endian == Endian::LITTLE_ENDIAN) {
            sign = 1;
            n = 0;
        } else {
            sign = -1;
            n = byteSize + startIdx;
        }

        constexpr auto mask = static_cast<std::byte>(0xFF);
        auto byteValue = static_cast<std::byte>(value);
        for (size_t i = startIdx; i < byteSize; i++, byteValue >>= 8) {
            (*bytes)[n + sign*i] = byteValue & mask;
        }
    }
}
