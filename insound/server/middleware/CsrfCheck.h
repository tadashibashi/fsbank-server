#pragma once
#include <insound/core/thirdparty/crow.hpp>

namespace Insound {

    class CsrfCheck {
    public:
        struct context
        {};

        void before_handle(crow::request &req, crow::response &res, context &ctx);

        void after_handle(crow::request &req, crow::response &res,
                          context &ctx);
    };

}
