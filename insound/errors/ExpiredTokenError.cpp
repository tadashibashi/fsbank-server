#include "ExpiredTokenError.h"
#include <string>
#include <Poco/JWT/Token.h>

Insound::ExpiredTokenError::ExpiredTokenError(const Poco::JWT::Token &token)
    : std::runtime_error(
          f("Token expired: exp={} cur={}",
            token.getExpiration().epochMicroseconds(),
            Poco::Timestamp().epochMicroseconds())
      ), token(token)
    {
    }
