#pragma once

#include <insound/core/errors/GlazeError.h>
#include <insound/core/thirdparty/fmt.hpp>
#include <insound/core/thirdparty/glaze.hpp>

#include <glaze/util/type_traits.hpp>

/**
 * Pass a type that specializes glz::meta in order to auto-format it
 * to a JSON string when passed into fmt library, f-strings, logs, etc.
 * @param  T - the type to format
 */
#define IN_JSON_FORMAT(T) template<> \
    struct ::fmt::formatter<T> { \
        template<typename ParseContext> \
        constexpr auto parse(ParseContext& ctx) { \
            return ctx.begin(); \
        } \
        template<typename FormatContext> \
        auto format(T const& obj, FormatContext& ctx) { \
            auto buffer = glz::write_json(obj); \
            return fmt::format_to(ctx.out(), "{}", buffer); \
        } \
    }
