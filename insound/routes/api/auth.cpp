#include "auth.h"
#include "crow/common.h"
#include "insound/log.h"
#include <insound/App.h>

using crow::HTTPMethod;

namespace Insound {
    Auth::Auth() : Router("api/auth") {}

    crow::Blueprint &Auth::config()
    {
        CROW_BP_ROUTE(bp, "/login/email")
            .methods(HTTPMethod::POST)
            (Auth::post_login);

        return bp;
    }

    crow::response Auth::post_login(const crow::request &req)
    {
        auto &user = App::getContext<UserAuth>(req).user;

        IN_LOG("User type in post route: {}", user.type);

        auto msg = crow::multipart::message(req);

        std::string password;
        std::string email;
        std::vector<std::string> files;

        // visit each part
        for (const auto &part : msg.part_map) {
            const auto &part_name = part.first;
            const auto &part_value = part.second;

            // get content info
            auto headers_it = part_value.headers.find("Content-Disposition");
            if (headers_it == part_value.headers.end())
            {
                CROW_LOG_ERROR << "No Content-Disposition found";
                return crow::response(400);
            }

            // check if part has a filename
            auto params_it = headers_it->second.params.find("filename");
            if (params_it == headers_it->second.params.end())
            {
                // if not, it's a text part
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
                // otherwise it's a file
                IN_LOG("part: {}, filename: {}", part_name, params_it->second);
                files.emplace_back(part_value.body);
            }
        }

        return crow::response(
            f("Password: {}, Email: {}, File: {}", password, email,
                (files.empty() ? "" : files[0])));
    }

} // namespace Insound
