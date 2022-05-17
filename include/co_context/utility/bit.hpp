#pragma once

#include <concepts>

namespace co_context {

template<std::integral T>
consteval T bit_top() noexcept {
    return T(-1) ^ (T(-1) >> 1);
}

template<std::integral T>
constexpr T lowbit(T x) noexcept {
    return x & (-x);
}

template<typename T>
concept trival_ptr = std::is_trivially_copyable_v<T>
                     && sizeof(T) == sizeof(std::uintptr_t);

} // namespace co_context