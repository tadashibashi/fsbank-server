#pragma once

#include <spdlog/fmt/fmt.h>

/**
 * Python f-string using fmt library
 */
template <typename... T>
FMT_NODISCARD FMT_INLINE auto sf(fmt::format_string<T...> format,
    T&&... args) -> std::string {
    return fmt::vformat(format, fmt::make_format_args(args...));
}
