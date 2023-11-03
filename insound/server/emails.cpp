#include "emails.h"
#include "insound/core/env.h"
#include <insound/core/jwt.h>
#include <insound/core/chrono.h>

#include <glaze/glaze.hpp>
#include <glaze/core/macros.hpp>

using namespace Insound::ChronoLiterals;

GLZ_META(Insound::Emails::EmailVerificationToken, _id, email);

namespace Insound::Emails {

    EmailStrings createVerificationStrings(std::string_view email, std::string_view id)
    {
        static std::string HOST_ADDRESS{requireEnv("HOST_ADDRESS")};

        auto token = EmailVerificationToken{
            ._id{id},
            .email{email}
        };

        auto jwt = Insound::Jwt::sign(token, 15_min);

        std::string link = HOST_ADDRESS + "/auth/verify?token=" + jwt;

        auto html = f(
R"html(<p>Hello,</p>
<p>Thank you for registering your account with Insound Audio!</p>
<p>To complete your registration, please visit this <a href="{0}">link</a>
within the next 15 minutes to verify your email address.</p>
<p>We hope you enjoy your new dynamic audio sharing service!</p>
<p>Best,</p>
<p>The Insound Audio Team</p>
)html", link);

        auto text = f(
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


    std::optional<EmailVerificationToken> verifyEmail(std::string_view tokenStr)
    {
        try {
            return Jwt::verify<EmailVerificationToken>(tokenStr);
        }
        catch(...)
        {
            return {};
        }
    }
}
