#include "UserContext.hpp"
#include <insound/jwt.h>

void Insound::UserContext::before_handle(crow::request &req,
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
        auto user = Jwt::verify<User>(tokenStr);

        ctx.user = user;
    } catch (...) {
        // something went wrong during verification, set user as guest
        ctx.user.type = User::Type::Guest;
    }


}

void Insound::UserContext::after_handle(crow::request &req,
                                         crow::response &res, context &ctx)
{
    std::cout << "User type: " << ctx.user.type << '\n';
}
