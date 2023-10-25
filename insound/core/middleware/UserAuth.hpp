#pragma once
#include <insound/core/schemas/User.h>

#include <crow/middleware.h>
#include <string>

namespace Insound {

    class UserAuth : crow::ILocalMiddleware
    {
    public:
        struct context
        {
            User user;
        };

        void before_handle(crow::request &req, crow::response &res, context &ctx);

        void after_handle(crow::request &req, crow::response &res,
                          context &ctx);

    private:
        std::string userLevel;
    };

}
