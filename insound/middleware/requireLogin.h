#pragma once


#include "Poco/JWT/Token.h"
#include "crow/middleware.h"
#include <Poco/JWT/JWT.h>
#include "Poco/JWT/Signer.h"

#include "../env.h"

namespace Insound {

    struct User {
        std::string type;
        std::string email;
    };

    struct RequireLogin : crow::ILocalMiddleware
    {
        struct context {
            User *user = nullptr;
        };

        void before_handle(crow::request &req, crow::response &res, context &ctx)
        {
            auto key = Insound::requireEnv("JWT_SECRET");
            Poco::JWT::Signer s;
            auto token = s.verify()

        }

        void after_handle(crow::request &req, crow::response &res, context &ctx)
        {

        }
    };

}
