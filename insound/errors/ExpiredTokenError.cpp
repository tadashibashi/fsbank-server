#include "ExpiredTokenError.h"
#include <string>
#include <Poco/JWT/Token.h>

Insound::ExpiredTokenError::ExpiredTokenError(const Poco::JWT::Token &token)
    : std::runtime_error(
          "Token expired: "
          "exp=" + std::to_string(token.getExpiration().epochMicroseconds()) + " "
          "cur=" + std::to_string(Poco::Timestamp().epochMicroseconds())
      ), token(token)
    {
    }
