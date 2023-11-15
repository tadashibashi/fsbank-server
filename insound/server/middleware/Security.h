/**
 * @file Security.h
 *
 * Contains crow middleware class `Security` for setting security headers.
 * (Alters defaults of Helmet middleware)
 */
#pragma once
#include <insound/core/thirdparty/crow.hpp>

namespace Insound {

    /**
     * This middleware sets various headers & server settings for security
     * purposes.
     */
    class Security {
    public:
        struct context
        {};

        void before_handle(crow::request &req, crow::response &res, context &ctx) {}

        void after_handle(crow::request &req, crow::response &res,
                          context &ctx);
    };

}
