#pragma once
#include <insound/core/json.h>
#include <insound/core/errors/JwtError.h>
#include <string>
#include <string_view>

namespace Insound::Jwt
{
    /**
     * Attempts to decode and verify a JSON web token.
     * @param  jwt - the base64-encoded token string
     * @returns json string of the payload
     *
     * @throws Jwt::Error if something went wrong during conversion
     *         or verification
     */
    std::string verify(std::string_view jwt);

    /**
     * Verify and decode a JSON web token string
     *
     * @param  jwt    the webtoken string
     * @param  buffer json out string
     * @return        ErrorCode - ErrorCode::OK on success,
     *                          and any other value on error.
     */
    Error::Code verify(std::string_view jwt, std::string &buffer);

    /**
     * Decode and verify a json web token, then verify its
     * payload, converting it to type `T`.
     *
     * @tname T   - type must have a specialization of glz::meta implemented
     * @param jwt - the json web token string
     *
     * @returns object of type T with its fields populated accordingly
     *
     * @throws GlazeError - if there was an error parsing token from JSON
     *         Jwt::Error - if there was a problem converting/verifying jwt
     *                      token string.
     */
    template<JSON::Specialized T, JSON::Opts O=JSON::Opts{
        .error_on_unknown_keys=false,
        .error_on_missing_keys=false
    }>
    inline T verify(std::string_view jwt)
    {
        std::string payloadStr = verify(jwt);

        T res;
        glz::context ctx{};
        auto error = glz::read<O>(res, payloadStr, ctx);

        if (error != glz::error_code::none)
            throw GlazeError(error, payloadStr);

        return res;
    }

    /**
     * Decode and verify a json web token then verify its payload, outputting
     * the result into type `T`.
     *
     * @tname T             - must have a specialization impl of glz::meta
     * @param jwt           - the json web token string
     * @param [out] output  - the object of type `T` to receive
     *
     * @return JWT error code: ErrorCode::OK on success, and any other value
     *         if there was an error converting or verifying the input token.
     *
     * @throws GlazeError if there was a problem deserializing the token into
     *                    type `T` object.
     */
    template<JSON::Specialized T, JSON::Opts O=JSON::Opts{
        .error_on_unknown_keys=false,
        .error_on_missing_keys=false
    }>
    inline Error::Code verify(std::string_view jwt, T &output)
    {
        try {
            auto obj = verify<T, O>(jwt);
        }
        catch (const Jwt::Error &e)
        {
            return e.code();
        }

        return Error::Code::OK;
    }


    /**
     * Sign a JSON web token with payload, which expires after the
     * duration specified.
     * @param   payloadStr - the payload as a json string
     * @param   expiresIn  - microseconds from now until expiration
     */
    std::string sign(std::string_view payloadStr, long long expiresIn);

    inline std::string sign(const std::string &payloadStr, long long expiresIn)
    {
        return sign(std::string_view(payloadStr), expiresIn);
    }


    /**
     * Signs a token with an object as a payload.
     *
     * @tname   T         - must be registered with glz::meta specialization
     * @param   payload   - the object to read data into the token from
     * @param   expiresIn - microseconds from now until expiration
     *
     * @returns a string of the json web token
     *
     * @throws system_error on any problem during jwt conversion/verification
     */
    template<JSON::Specialized T>
    inline std::string sign(const T &payload, long long expiresIn)
    {
        auto payloadStr = glz::write_json(payload);

        return sign(std::string_view(payloadStr), expiresIn);
    }

    /**
     * Signs a token with an object as the payload
     *
     * @tname T         - must be registered with glz::meta specialization
     * @param payload   - the object to read from
     * @param expiresIn - microseconds from now until token expiration
     * @param output    - output token to receive
     *
     * @return ErrorCode - ErrorCode::OK on success and eny other value on
     *                     an error.
     */
    template <JSON::Specialized T>
    inline Error::Code sign(const T&payload, long long expiresIn, std::string &output)
    {
        auto payloadStr = glz::write_json(payload);

        try {
            output = std::move(sign(std::string_view(payloadStr), expiresIn));
        }
        catch (const std::system_error &e)
        {
            return Error::Code::VerificationFailure;
        }
    }
}
