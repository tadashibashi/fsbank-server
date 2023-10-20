#include "auth.h"
#include "crow/common.h"
#include "insound/log.h"
#include <insound/app.h>

using crow::HTTPMethod;

namespace Insound::Auth {
    static auto auth = crow::Blueprint("api/auth");

    crow::Blueprint &config()
    {
        CROW_BP_ROUTE(auth, "/login/email")
            .methods(HTTPMethod::POST)
            (post_login);

        return auth;
    }

    crow::response post_login(const crow::request &req)
    {
        auto &userCtx = getContext<UserAuth>(req);

        IN_LOG("User type in post route: {}", userCtx.user.type);

        auto msg = crow::multipart::message(req);

        std::string password;
        std::string email;
        std::vector<std::string> files;

        // visit each part
        for (const auto &part : msg.part_map) {
            const auto &part_name = part.first;
            const auto &part_value = part.second;

            // get filename
            auto headers_it = part_value.headers.find("Content-Disposition");
            if (headers_it == part_value.headers.end())
            {
                CROW_LOG_ERROR << "No Content-Disposition found";
                return crow::response(400);
            }

            // check if it has a file name
            auto params_it = headers_it->second.params.find("filename");
            if (params_it == headers_it->second.params.end())
            {
                // It's a text part
                IN_LOG("part: {}, value: {}", part_name, part_value.body);
                if (part_name == "password")
                {
                    password = part_value.body;
                }
                else if (part_name == "email")
                {
                    email = part_value.body;
                }
            }
            else
            {
                // It's a file
                IN_LOG("part: {}, filename: {}", part_name, params_it->second);
                files.emplace_back(part_value.body);
            }
        }

        return crow::response("Password: " + password + ", Email: " + email + ", File: " + (files.empty() ? "" : files[0]) );
    }
}
