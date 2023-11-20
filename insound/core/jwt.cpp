#include "jwt.h"
#include <insound/core/errors/GlazeError.h>
#include <insound/core/errors/JwtError.h>
#include <insound/core/settings.h>
#include <insound/core/thirdparty/glaze.traits.h>

#include <jwt-cpp/jwt.h>

#include <cstdint>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <utility>

namespace Insound::Jwt
{

    std::string verify(std::string_view token)
    {
        static const auto verifier = jwt::verify<glaze_traits>()
            .allow_algorithm(jwt::algorithm::hs256{
                Settings::jwtSecret().data()
            });

        std::optional<jwt::decoded_jwt<glaze_traits>> decoded;
        try {
            decoded = jwt::decode<glaze_traits>(token.data());
        }
        catch (const std::invalid_argument &e)
        {
            throw Jwt::Error(Jwt::Error::Code::BadTokenFormat);
        }
        catch (const std::runtime_error &e)
        {
            throw Jwt::Error(Jwt::Error::Code::ConversionFailure);
        }

        try {
            verifier.verify(*decoded);
        }
        catch (const std::system_error &e)
        {
            throw Jwt::Error(Jwt::Error::Code::VerificationFailure, e.what());
        }

        return decoded->get_payload();
    }

    Error::Code verify(std::string_view token, std::string &buffer)
    {
        try {
            buffer = std::move(verify(token));
        }
        catch (Error error)
        {
            return error.code();
        }

        return Error::Code::OK;
    }

    std::string sign(std::string_view payloadStr, long long duration)
    {
        using std::chrono::system_clock;
        using std::chrono::microseconds;

        static auto algo = jwt::algorithm::hs256{Settings::jwtSecret().data()};

        auto now = system_clock::now();
        auto expires_at = now + microseconds{duration};

        glz::json_t payload;
        auto err = glz::read_json<glz::json_t>(payload, payloadStr);
        if (err.ec != glz::error_code::none)
            throw GlazeError(err, payloadStr);

        auto builder = jwt::builder<glaze_traits>(
                payload.as<glz::json_t::object_t>())
            .set_type("JWT")
            .set_issued_at(now)
            .set_expires_at(expires_at);

        return builder.sign(algo);
    }
}
