#include "jwt.h"

#include <insound/env.h>
#include <insound/errors/ExpiredTokenError.h>

#include <Poco/JSON/Parser.h>
#include <Poco/JWT/Serializer.h>
#include <Poco/JWT/Signer.h>

#include <sstream>

namespace Insound::Jwt
{
    std::string verify(const std::string &jwt)
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

        std::stringstream stream;
        token.payload().stringify(stream);
        return stream.str();
    }

    std::string sign(const std::string &payloadStr, long long duration)
    {
        // Parse payload into object
        Poco::JSON::Parser parser;
        auto payload = parser.parse(payloadStr).extract<Poco::JSON::Object::Ptr>();

        // Create the signer
        static const auto JWT_SECRET = requireEnv("JWT_SECRET");
        Poco::JWT::Signer signer(JWT_SECRET);

        // Create the token
        Poco::JWT::Token token;
        token.setType("JWT");

        token.payload() = *payload;

        // Set expiration
        const auto cur = Poco::Timestamp().epochMicroseconds();
        const auto exp = Poco::Timestamp( cur + duration );
        token.setExpiration( exp );

        // Sign & return the token
        return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
    }
}
