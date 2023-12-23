#include "emails.h"
#include "insound/core/settings.h"
#include <insound/core/jwt.h>
#include <insound/core/chrono.h>

#include <glaze/glaze.hpp>
#include <glaze/core/macros.hpp>

using namespace Insound::ChronoLiterals;

GLZ_META(Insound::Emails::EmailVerificationToken, _id, email);

namespace Insound::Emails {

    EmailStrings createVerificationStrings(std::string_view email,
        std::string_view id)
    {
        auto token = EmailVerificationToken{
            ._id{id},
            .email{email}
        };

        auto jwt = Insound::Jwt::sign(token, 15_min);
        auto link = sf("{}/auth/verify?token={}", Settings::hostAddress(), jwt);

        auto html = sf(
R"html(<p>Hello,</p>
<p>Thank you for registering your account with Insound Audio!</p>
<p>To complete your registration, please visit this <a href="{0}">link</a>
within the next 15 minutes to verify your email address.</p>
<p>We hope you enjoy your new dynamic audio sharing service!</p>
<p>Best,</p>
<p>The Insound Audio Team</p>
)html", link);

        auto text = sf(
R"(Hello,

Thank you for registering your account with Insound Audio!

To complete your registration, please visit this link within the next 15 minutes to verify your email address:

{0}

We hope you enjoy your new dynamic audio sharing service!


Best,

The Insound Audio Team
)", link);

        return {
            .html = html,
            .text = text,
        };
    }


    VerificationResult verifyEmail(std::string_view tokenStr, EmailVerificationToken *outToken)
    {
        try {
            *outToken = Jwt::verify<EmailVerificationToken>(tokenStr);
            return VerificationResult::OK;
        }
        catch(const Jwt::Error &error)
        {
            switch(error.code())
            {
            case Jwt::Error::Code::BadTokenFormat:
            case Jwt::Error::Code::ConversionFailure:
            case Jwt::Error::Code::VerificationFailure:
                return VerificationResult::InvalidToken;

            case Jwt::Error::Code::TokenExpired:
                return VerificationResult::TokenExpired;

            case Jwt::Error::Code::OK: // this is unusual, should never occur
                throw error; // rethrow
            }
        }
    }
}
