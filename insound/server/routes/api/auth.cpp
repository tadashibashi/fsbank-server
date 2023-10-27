#include "auth.h"
#include "insound/core/mongo/Model.h"
#include "insound/core/password.h"
#include <crow/common.h>
#include <crow/middlewares/cookie_parser.h>

#include <insound/server/Server.h>

#include <insound/core/util.h>

using SameSitePolicy = crow::CookieParser::Cookie::SameSitePolicy;
namespace Insound {
    Auth::Auth() : Router("api/auth") {}

    void Auth::init()
    {
        CROW_BP_ROUTE(bp, "/login/email")
            .methods("POST"_method)
            (Auth::post_login);

        CROW_BP_ROUTE(bp, "/check")
            .methods("GET"_method)
            (Auth::get_check);

        CROW_BP_ROUTE(bp, "/create/email")
            .methods("POST"_method)
            (Auth::post_create);

        CROW_BP_CATCHALL_ROUTE(bp)([]() {return "hey";});
    }

    crow::response Auth::get_check(const crow::request &req)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);
        auto &user = Server::getContext<UserAuth>(req).user;

        auto fingerprint = cookies.get_cookie("fingerprint");
        if (compare(fingerprint, user.fingerprint)) {

        }

        return crow::response("Check auth GET!");
    }

    crow::response &Auth::post_login(const crow::request &req, crow::response &res)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);

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
                res.code = 400;
                res.body = "Invalid header, no Content-Disposition found";
                return res;
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

        // check for user

        Mongo::Model<User> UserModel;
        auto userRes = UserModel.findOne({"email", email});
        if (!userRes)
        {
            res.code = 401;
            res.body = "Could not find a user with that email.";
            return res;
        }

        auto &user = userRes.value();

        if (!compare(user.body.password, password))
        {
            res.code = 401;
            res.body = "Invalid password.";
            return res;
        }

        UserToken token;
        token.username = user.body.username;
        token.displayName = user.body.displayName;
        token.email = user.body.email;
        token.type = user.body.type;

        const auto fingerprint = genHexString();
        cookies.set_cookie("fingerprint", fingerprint)
            .httponly()
            .same_site(SameSitePolicy::Strict)
            .max_age(60 * 60 * 24 * 14)
            .path("/");
        token.fingerprint = hash(fingerprint);

        res.body = glz::write_json(token);
        res.code = 200;
        return res;
    }

    crow::response &Auth::post_create(const crow::request &req, crow::response &res)
    {

    }

} // namespace Insound
