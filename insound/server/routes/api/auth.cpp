#include "auth.h"
#include <insound/core/chrono.h>
#include <insound/core/jwt.h>

#include <insound/core/mongo/Model.h>
#include <insound/core/MultipartMap.h>
#include <insound/core/password.h>
#include <insound/core/util.h>
#include <insound/server/Server.h>

#include <crow/common.h>
#include <crow/middlewares/cookie_parser.h>

using namespace Insound::ChronoLiterals;

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
    }

    void Auth::get_check(const crow::request &req, crow::response &res)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);
        auto &user = Server::getContext<UserAuth>(req).user;

        res.add_header("Content-Type", "application/json");

        auto fingerprint = cookies.get_cookie("fingerprint");

        if (user.isAuthorized() && compare(fingerprint, user.fingerprint))
        {
            res.body = R"({"auth":true})";
        }
        else
        {
            res.body = R"({"auth":false})";
        }

        res.end();
    }

    void Auth::post_login(const crow::request &req, crow::response &res)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);
        auto body = MultipartMap::from(req);

        // Required fields to collect
        std::string email;
        std::string password;

        try {
            email = body.fields.at("email");
            password = body.fields.at("password");
        }
        catch(...)
        {
            res.code = 400;
            res.add_header("Content-Type", "application/json");
            res.end(R"({"error":"Missing fields."})");
            return;
        }

        // Check honeypot
        if (!body.fields["password2"].empty())
        {
            res.code = 400;
            res.add_header("Content-Type", "application/json");
            return res.end(R"({"error":"Invalid request."})");
        }

        // Check if user with email exists
        Mongo::Model<User> UserModel;
        auto userRes = UserModel.findOne({"email", email});
        if (!userRes)
        {
            res.code = 401;
            res.add_header("Content-Type", "application/json");
            return res.end(R"({"error":"Could not find a user with that email."})");
        }

        // Check password
        auto &user = userRes.value();
        if (!compare(password, user.body.password))
        {
            res.code = 401;
            res.add_header("Content-Type", "application/json");
            return res.end(R"({"error":"Invalid password."})");
        }

        try {
            // Create user token & fingerprint
            const auto fingerprint = genHexString();

            UserToken token;
            token.username = user.body.username;
            token.displayName = user.body.displayName;
            token.email = user.body.email;
            token.type = user.body.type;
            token.fingerprint = hash(fingerprint);

            // Sign the token
            res.body = Jwt::sign(token, 2_w);

            // Done, commit fingerprint cookie
            cookies.set_cookie("fingerprint", fingerprint)
                .httponly()
                .same_site(SameSitePolicy::Strict)
                .max_age(60 * 60 * 24 * 14) // two weeks
                .path("/");

            res.set_header("Content-Type", "text/plain");
            res.code = 200;
            res.end();
        }
        catch(const std::exception &e)
        {
            IN_ERR(e.what());
            res.code = 500;
            res.end(R"({"error":"Internal Error."})");
        }
        catch(...)
        {
            IN_ERR("Unknown error occurred");
            res.code = 500;
            res.end(R"({"error":"Internal Error."})");
        }
    }

    void Auth::post_create(const crow::request &req, crow::response &res)
    {
        auto body = MultipartMap::from(req);
        res.add_header("Content-Type", "application/json");

        // Check honeypot
        if (!body.fields["username2"].empty())
        {
            res.code = 200;
            return res.end();
        }

        // Get fields
        std::string email;
        std::string password;
        try {
            email = body.fields.at("email");
            password = body.fields.at("password");

            if (password != body.fields.at("password2"))
            {
                res.code = 400;
                return res.end(R"({"error":"Passwords mismatch."})");
            }
        }
        catch (...)
        {
            res.code = 400;
            return res.end(R"({"error":"Missing field."})");
        }

        // Check if user already exists
        auto UserModel = Mongo::Model<User>();
        auto user = UserModel.findOne({"email", email});
        if (user)
        {
            res.code = 400;
            return res.end(R"({"error":"User with email account already exists."})");
        }

        // Create new user
        User newUser;
        newUser.email = email;
        newUser.password = hash(password);
        newUser.type = User::Type::User;

        auto doc = UserModel.insertOne(newUser);
        if (!doc)
        {
            // something went wrong...
            res.code = 500;
            return res.end(R"({"error":"Internal Error."})");
        }

        // TODO: send verification email here

        res.code = 200;
        return res.end(R"({"result": "Success"})");
    }

} // namespace Insound
