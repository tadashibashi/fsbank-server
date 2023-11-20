#pragma once
#include <insound/core/settings.h>
#include <insound/core/errors/JwtError.h>
#include <insound/core/thirdparty/glaze.hpp>

#include <jwt-cpp/jwt.h>
#include <typeindex>

namespace Insound
{
    struct glaze_traits
    {
        using value_type = glz::json_t;
        using object_type = glz::json_t::object_t;
        using array_type = glz::json_t::array_t;
        using string_type = std::string;
        using number_type = double;
        using integer_type = int64_t;
        using boolean_type = bool;

        static jwt::json::type get_type(const value_type &val)
        {
            using jwt::json::type;
            auto type_hash = std::type_index(typeid(val.data)).hash_code();

            if (std::holds_alternative<object_type>(val.data))
                return type::object;
            else if (std::holds_alternative<array_type>(val.data))
                return type::array;
            else if (std::holds_alternative<string_type>(val.data))
                return type::string;
            else if (std::holds_alternative<number_type>(val.data))
                return type::number;
            else if (std::holds_alternative<boolean_type>(val.data))
                return type::boolean;

            throw "JSON type is not recognized.";
        }

        static object_type as_object(const value_type &val)
        {
            return val.as<object_type>();
        }

        static array_type as_array(const value_type &val)
        {
            return val.as<array_type>();
        }

        static string_type as_string(const value_type &val)
        {
            return val.as<string_type>();
        }

        static number_type as_number(const value_type &val)
        {
            return val.as<number_type>();
        }

        static integer_type as_integer(const value_type &val)
        {
            return val.as<integer_type>();
        }

        static boolean_type as_boolean(const value_type &val)
        {
            return val.as<boolean_type>();
        }

        static bool parse(value_type &val, string_type str)
        {
            return glz::read_json(val, str) == glz::error_code::none;
        }

        static string_type serialize(const value_type &val)
        {
            return glz::write_json(val);
        }
    };
}
