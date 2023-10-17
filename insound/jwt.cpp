#include "jwt.h"
#include "Poco/JWT/Serializer.h"
#include "Poco/JWT/Signer.h"
#include "insound/env.h"

namespace Insound::Jwt
{

    Poco::JSON::Object deserialize(const std::string &jwt)
    {
        static const auto JWT_SECRET = requireEnv("JWT_SECRET");
        Poco::JWT::Signer signer(JWT_SECRET);

        return signer.verify(jwt).payload();
    }

    std::string serialize(const Poco::JSON::Object &payload)
    {
        static const auto JWT_SECRET = requireEnv("JWT_SECRET");
        Poco::JWT::Token token;
        token.setType("JWT");
        token.payload() = Poco::JSON::Object(payload);
        Poco::JWT::Signer signer(JWT_SECRET);

        return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
    }
}
