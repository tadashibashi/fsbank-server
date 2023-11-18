#include "CsrfCheck.h"
#include <insound/core/settings.h>
#include <insound/core/util.h>

#include <insound/server/Server.h>
#include <insound/core/thirdparty/crow.hpp>

namespace Insound
{
    void CsrfCheck::before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        static std::string CSRF_SECRET_KEY{requireEnv("CSRF_SECRET_KEY")};
        static bool CSRF_ALLOW_BYPASS{requireEnv("CSRF_ALLOW_BYPASS") == "true"};

        if (!Settings::isProd()) // deactivate csrf protection in debug mode
            return;

        // Only check for non-get requests. Get requests should not perform any
        // mutable task such as posting data, altering databases, etc.
        if (req.method != crow::HTTPMethod::GET)
        {
            auto csrftoken = req.get_header_value("X-CSRF-TOKEN");
            auto &cookies = Server::getContext<crow::CookieParser>(req);
            auto cookie = cookies.get_cookie("csrftoken");

            bool authorized;

            if (CSRF_ALLOW_BYPASS)
            {
                authorized =  csrftoken == CSRF_SECRET_KEY ||
                    (!cookie.empty() && cookie == csrftoken);
            }
            else
            {
                authorized = !cookie.empty() && cookie == csrftoken;
            }

            if (!authorized)
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
