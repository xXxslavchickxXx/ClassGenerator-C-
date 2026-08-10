#pragma once

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
    }