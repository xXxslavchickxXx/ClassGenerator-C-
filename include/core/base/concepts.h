#pragma once

template<typename U, typename... T>
constexpr bool is_one_of_v = (std::is_same_v<std::remove_cvref_t<U>, std::remove_cvref_t<T>> || ...);