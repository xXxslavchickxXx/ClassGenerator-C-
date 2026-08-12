#pragma once

#include <ostream>

// | - или
// & - и
// ^ - xor
// ~ - не

#define ENABLE_BITMASK_OPERATORS(T) \
    inline constexpr T operator|(T a, T b) { \
        using U = std::underlying_type_t<T>; \
        return static_cast<T>(static_cast<U>(a) | static_cast<U>(b)); \
    } \
    inline constexpr T operator&(T a, T b) { \
        using U = std::underlying_type_t<T>; \
        return static_cast<T>(static_cast<U>(a) & static_cast<U>(b)); \
    } \
    inline constexpr T operator^(T a, T b) { \
        using U = std::underlying_type_t<T>; \
        return static_cast<T>(static_cast<U>(a) ^ static_cast<U>(b)); \
    } \
    inline constexpr T operator~(T a) { \
        using U = std::underlying_type_t<T>; \
        return static_cast<T>(~static_cast<U>(a)); \
    } \
    inline constexpr T& operator|=(T& lhs, T rhs) { \
        using U = std::underlying_type_t<T>; \
        lhs = static_cast<T>(static_cast<U>(lhs) | static_cast<U>(rhs)); \
        return lhs; \
    } \
    inline constexpr T& operator&=(T& lhs, T rhs) { \
        using U = std::underlying_type_t<T>; \
        lhs = static_cast<T>(static_cast<U>(lhs) & static_cast<U>(rhs)); \
        return lhs; \
    } \
    inline constexpr T& operator^=(T& lhs, T rhs) { \
        using U = std::underlying_type_t<T>; \
        lhs = static_cast<T>(static_cast<U>(lhs) ^ static_cast<U>(rhs)); \
        return lhs; \
    } \
    inline std::ostream& operator<<(std::ostream& os, const T& val) { \
        auto value = static_cast<std::underlying_type_t<T>>(val); \
        \
        os << "0b"; \
        for (int i = sizeof(value) * 8 - 1; i >= 0; --i) { \
            os << ((value >> i) & 1); \
        } \
        \
        return os; \
    }