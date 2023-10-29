#include "jwt.h"

#include <insound/core/env.h>

#include <glaze/glaze.hpp>

#include <jwt-cpp/jwt.h>

#include <cstdint>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <variant>

namespace Insound::Jwt
{
    struct glaze_traits {
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

    std::string verify(const std::string &token)
    {
        static auto JWT_SECRET = requireEnv("JWT_SECRET");
        auto decoded = jwt::decode<glaze_traits>(token);
        auto verifier = jwt::verify<glaze_traits>()
            .allow_algorithm(jwt::algorithm::hs256{JWT_SECRET.data()});
        verifier.verify(decoded);

        return decoded.get_payload();
    }

    std::string sign(const std::string &payloadStr, long long duration)
    {
        using std::chrono::system_clock;
        using std::chrono::microseconds;

        auto now = system_clock::now();
        auto expires_at = now + microseconds{duration};
        static auto JWT_SECRET = requireEnv("JWT_SECRET");

        glz::json_t payload;
        auto err = glz::read_json<glz::json_t>(payload, payloadStr);
        if (err.ec != glz::error_code::none)
            throw f("Glaze error: {}", (int)err.ec);

        auto builder = jwt::builder<glaze_traits>(
                payload.as<glz::json_t::object_t>())
            .set_type("JWT")
            .set_issued_at(now)
            .set_expires_at(expires_at);

        return builder.sign(jwt::algorithm::hs256{JWT_SECRET.data()});
    }
}
