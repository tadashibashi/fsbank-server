#include "JwtError.h"

namespace Insound::Jwt {
    std::string_view Error::errorString(Code ec)
    {
        switch(ec)
        {
        case Code::OK: return "No errors";
        case Code::BadTokenFormat: return "Bad input token string";
        case Code::ConversionFailure:
            return "Error during token conversion";
        case Code::VerificationFailure:
            return "Token converted, but failed verification";
        default:
            return "Unknown error code";
        }
    }
}
