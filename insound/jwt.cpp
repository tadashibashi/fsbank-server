#include "jwt.h"

#include <insound/env.h>
#include <insound/errors/ExpiredTokenError.h>
#include <Poco/JWT/Serializer.h>
#include <Poco/JWT/Signer.h>

namespace Insound::Jwt
{
    Poco::JSON::Object verify(const std::string &jwt)
    {
        // Create the signer
        static const auto JWT_SECRET = requireEnv("JWT_SECRET");
        Poco::JWT::Signer signer(JWT_SECRET);

        auto token = signer.verify(jwt);

        // Check for expiration
        const auto exp = token.getExpiration();
        if (exp < Poco::Timestamp()) {
            throw ExpiredTokenError(token);
        }

        return token.payload();
    }

    std::string sign(const Poco::JSON::Object &payload, long long duration)
    {
        // Create the signer
        static const auto JWT_SECRET = requireEnv("JWT_SECRET");
        Poco::JWT::Signer signer(JWT_SECRET);

        // Create the token
        Poco::JWT::Token token;
        token.setType("JWT");

        // Copy payload into token payload
        for (auto [k, v] : payload)
            token.payload().set(k, v);

        // Set expiration
        const auto cur = Poco::Timestamp().epochMicroseconds();
        const auto exp = Poco::Timestamp( cur + duration );
        token.setExpiration( exp );

        // Sign & return the token
        return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
    }
}
