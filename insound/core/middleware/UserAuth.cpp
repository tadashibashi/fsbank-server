#include "UserAuth.h"

#include <insound/core/jwt.h>
#include <insound/core/log.h>

#include <insound/core/schemas/User.json.h>

void Insound::UserAuth::before_handle(crow::request &req,
    crow::response &res, context &ctx)
{
    auto auth = req.get_header_value("Authorization");
    if (!auth.starts_with("Bearer "))
    {
        ctx.user.type = User::Type::Guest;
        return;
    }

    auto tokenStr = auth.substr(7);

    try {
        auto user = Jwt::verify<UserToken>(tokenStr);

        ctx.user = user;
    } catch (...) {
        // Something went wrong during verification, set user as guest
        ctx.user.type = User::Type::Guest;
    }


}

void Insound::UserAuth::after_handle(crow::request &req, crow::response &res,
    context &ctx)
{

}
