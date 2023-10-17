#pragma once
#include <Poco/JWT/Serializer.h>

namespace Insound::Jwt
{
    /**
     * Attempts to deserialize a JWT.
     * Throws a SignatureVerificationException, on failure.
     * @param  jwt - the token
     */
    Poco::JSON::Object deserialize(const std::string &jwt);

    std::string serialize(const Poco::JSON::Object &payload);
}
