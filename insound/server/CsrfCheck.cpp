#include "CsrfCheck.h"
#include <insound/core/util.h>

#include <insound/server/Server.h>
#include <insound/core/thirdparty/crow.hpp>

namespace Insound {
    void CsrfCheck::before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        static std::string CSRF_SECRET_KEY{requireEnv("CSRF_SECRET_KEY")};

        // Only check for non-get requests. Get requests should not perform any
        // mutable task such as posting data, altering databases, etc.
        if (req.method != crow::HTTPMethod::GET)
        {
            auto csrftoken = req.get_header_value("X-CSRF-TOKEN");
            auto &cookies = Server::getContext<crow::CookieParser>(req);
            auto cookie = cookies.get_cookie("csrftoken");

            if (cookie.empty() ||
                (csrftoken != CSRF_SECRET_KEY && csrftoken != cookie))
            {
                res.code = 403;
                return res.end("Unauthorized.");
            }
        }
    }

    void CsrfCheck::after_handle(crow::request &req, crow::response &res, context &ctx)
    {

    }
}
