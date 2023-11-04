#pragma once
#include "insound/core/errors/GlazeError.h"
#include <glaze/glaze.hpp>
#include <string>
#include <string_view>

namespace Insound::Jwt
{
    /**
     * Attempts to decode and verify a JSON web token.
     * @param  jwt - the base64-encoded token string
     * @returns json string of the payload
     */
    std::string verify(std::string_view jwt);


    /**
     * Attempts to decode and verify a json web token, then verifies its
     * payload and populates it into type `T`. Throws exception if token
     * verification fails, or if fields from type `T` are missing, or of the
     * wrong type, etc.
     *
     * @tname T   - type must have a specialization of glz::meta implemented
     * @param jwt - the json web token string
     *
     * @returns object of type T with its fields populated accordingly
     */
    template<typename T>
    T verify(std::string_view jwt)
    {
        std::string payloadStr = verify(jwt);

        T res;
        glz::context ctx{};
        auto error = glz::read<glz::opts{
            .error_on_unknown_keys=false,
            .error_on_missing_keys=true,
        }>(res, payloadStr, ctx);

        if (error != glz::error_code::none)
            throw GlazeError(error, payloadStr);

        return res;
    }


    /**
     * Sign a JSON web token with payload, which expires after the
     * duration specified.
     * @param   payloadStr - the payload as a json string
     * @param   expiresIn  - microseconds from now until expiration
     */
    std::string sign(std::string_view payloadStr, long long expiresIn);


    /**
     * Signs a token with an object as a payload.
     *
     * @tname   T         - must be registered with glz::meta specialization
     * @param   payload   - the object to read data into the token from
     * @param   expiresIn - microseconds from now until expiration
     *
     * @returns a string of the json web token
     */
    template<typename T>
    std::string sign(const T &payload, long long expiresIn)
    {
        auto payloadStr = glz::write_json(payload);

        return sign(payloadStr, expiresIn);
    }
}
