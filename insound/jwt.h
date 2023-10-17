#pragma once
#include <Poco/JWT/Serializer.h>

namespace Insound::Jwt
{
    /**
     * Attempts to decode and verify a JSON web token.
     * Throws a Poco::SignatureVerificationException, on verification failure,
     * based on token validity, and a Insound::ExpiredTokenError if the token
     * was expired.
     * @param  jwt - the base64-encoded token string
     */
    Poco::JSON::Object verify(const std::string &jwt);

    /**
     * Sign a JSON web token with payload, which expires after the
     * duration specified.
     * @param   payload - the payload
     * @param duration - microseconds from now
     */
    std::string sign(const Poco::JSON::Object &payload, long long duration);
}
