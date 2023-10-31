#pragma once
#include <crow/middleware.h>

namespace Insound {

    class Helmet : crow::ILocalMiddleware
    {
    public:
        struct context
        {
            std::string nonce;
        };

        void before_handle(crow::request &req, crow::response &res, context &ctx);

        void after_handle(crow::request &req, crow::response &res,
                          context &ctx);
    };

}
