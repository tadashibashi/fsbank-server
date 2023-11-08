#pragma once

#include <insound/core/errors/GlazeError.h>
#include <insound/core/format.h>
#include <insound/core/thirdparty/glaze.hpp>

#include <glaze/util/type_traits.hpp>
#include <glaze/api/std/array.hpp>
#include <glaze/core/common.hpp>

// json-serializable std lib built-in templates
#include <array>
#include <concepts>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define IN_JSON_META(name, ...) GLZ_META(name, __VA_ARGS__); \
    GLZ_FORMAT(name)
#define IN_JSON_LOCAL_META(name, ...) GLZ_LOCAL_META(name, __VA_ARGS__)

namespace Insound::JSON {

    // ===== type traits ======================================================

    // Char array

    template <class> struct is_bounded_char_array : std::false_type {};
    template <size_t N>
    struct is_bounded_char_array<char[N]> : std::true_type {};

    /**
     * A class that is JSON-serializable
     */
    template <typename T>
    concept Serializable =
        glz::is_specialization_v<glz::meta<T>, glz::meta> ||
        requires {T::glaze;} ||
        std::is_same_v<std::string, T> ||
        std::is_same_v<std::string_view, T> ||
        glz::is_specialization_v<T, glz::detail::Object> ||
        glz::is_specialization_v<T, glz::detail::Array> ||
        is_bounded_char_array<T>::value ||
        glz::is_span_v<T, std::array> ||
        glz::is_span_v<T, std::span> ||
        glz::is_specialization_v<T, std::deque> ||
        glz::is_specialization_v<T, std::forward_list> ||
        glz::is_specialization_v<T, std::list> ||
        glz::is_specialization_v<T, std::map> ||
        glz::is_specialization_v<T, std::unordered_map> ||
        glz::is_specialization_v<T, std::set> ||
        glz::is_specialization_v<T, std::unordered_set> ||
        glz::is_specialization_v<T, std::vector> ||
        glz::is_specialization_v<T, std::tuple>;

    /**
     * A class that has specialized glz::meta either globally or locally
     */
    template <typename T>
    concept Specialized =
        glz::is_specialization_v<glz::meta<T>, glz::meta> ||
        requires {T::glaze;};


    // ===== Type synonyms  ===================================================
    using Opts = glz::opts;
    using ErrorCode = glz::error_code;
    using ParseError = glz::parse_error;


    // ===== API ==============================================================

    /**
     * Stringify a JSON-serializable object
     *
     * @param obj - object to serialize
     *
     * @return JSON string of the serializable object.
     */
    template <Serializable T>
    inline
    std::string stringify(const T &obj)
    {
        return glz::write_json(obj);
    }

    /**
     * Parse JSON-serializable object into a JSON string.
     *
     * Default options are loose, allowing for unknown and missing keys.
     * This enables structs to set their default settings when a field is
     * missing, while ignoring irrelevant keys. These settings are
     * configurable setting the Opts literal in tparam `O`.
     *
     * Throws a GlazeError if there was an issue parsing.
     *
     * @param json - json string
     *
     * @return parsed object of type T
     */
    template <Serializable T, Opts O=Opts{
        .error_on_unknown_keys=false,
        .error_on_missing_keys=false
    }>
    inline
    T parse(std::string_view json)
    {
        T obj;
        glz::parse_error err = glz::read<O>(obj, json);
        if (err.ec != glz::error_code::none)
            throw GlazeError(err, json);
        return obj;
    }

    /**
     * Parse JSON-serializable object into a JSON string.
     *
     * @return JSON::ParseError - JSON::ErrorCode::none on okay, and anything
     *                    else if there is an error.
     */
    template<Serializable T, Opts O=Opts{
        .error_on_unknown_keys=false,
        .error_on_missing_keys=false
    }>
    inline
    ParseError parse(std::string_view json, T &obj)
    {
        return glz::read<O>(json, obj);
    }
}
