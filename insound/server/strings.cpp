#include "strings.h"
#include "insound/core/env.h"
#include <insound/core/jwt.h>
#include <insound/core/chrono.h>

using namespace Insound::ChronoLiterals;

namespace Insound::Emails {

    EmailStrings verification(const Insound::UserToken &token)
    {
        static std::string HOST_ADDRESS{requireEnv("HOST_ADDRESS")};

        auto jwt = Insound::Jwt::sign(token, 15_min);
        std::string link = HOST_ADDRESS + "/auth/verify?token=" + jwt;

        auto html = f(
R"html(<p>Hello {0},</p>
<p>Thank you for registering your account with Insound Audio!</p>
<p>To complete your registration, please visit this <a href="{1}">link</a>
to verify your email address.</p>
<p>We hope you enjoy your new dynamic audio sharing service!</p>
<p>Best,</p>
<p>The Insound Audio Team</p>
)html", token.displayName, link);

        auto text = f(
R"(Hello {0},

Thank you for registering your account with Insound Audio!

To complete your registration, please visit this link to verify your email address:

{1}

We hope you enjoy your new dynamic audio sharing service!


Best,

The Insound Audio Team
)", token.displayName, link);

        return {
            .html = html,
            .text = text,
        };
    }

}
