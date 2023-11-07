#include "auth.h"
#include "insound/core/HttpStatus.h"
#include "insound/core/email.h"
#include "insound/core/json.h"
#include "insound/core/regex.h"
#include "insound/core/schemas/FormErrors.json.h"
#include "insound/server/emails.h"
#include <insound/core/chrono.h>
#include <insound/core/jwt.h>

#include <insound/core/schemas/User.json.h>

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

        CROW_BP_ROUTE(bp, "/verify")
            .methods("POST"_method)
            (Auth::post_verify);
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
            res.code = 401;
            res.body = R"({"auth":false})";
        }

        res.end();
    }

    Response Auth::post_login(const crow::request &req)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);
        auto body = MultipartMap::from(req);

        // Required fields to collect
        std::string email;
        std::string password;

        auto errors = FormErrors();

        try {
            email = body.fields.at("email");
            password = body.fields.at("password");
        }
        catch(...)
        {
            if (email.empty())
                errors.append("email", "Missing field.");
            if (password.empty())
                errors.append("password", "Missing field.");

            return Response::json(errors, HttpStatus::BadRequest);
        }

        // Check honeypot
        if (!body.fields["password2"].empty())
        {
            errors.append("password2", "Field should be empty.");
            return Response::json(errors, HttpStatus::BadRequest);
        }

        if (!std::regex_match(email, Regex::email))
        {
            errors.append("email", "Invalid email address.");
            return Response::json(errors, HttpStatus::BadRequest);
        }

        // Check if user with email exists
        Mongo::Model<User> UserModel;
        auto userRes = UserModel.findOne({"email", email});
        if (!userRes)
        {
            errors.append("email", "Could not find a user with that address.");
            return Response::json(errors, HttpStatus::Unauthorized);
        }

        // Check password
        auto &user = userRes.value();
        if (!compare(password, user.body.password))
        {
            errors.append("password", "Invalid password.");
            return Response::json(errors, HttpStatus::Unauthorized);
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
            auto jwt = Jwt::sign(token, 2_w);

            // Done, commit fingerprint cookie
            cookies.set_cookie("fingerprint", fingerprint)
                .httponly()
                .same_site(SameSitePolicy::Strict)
                .max_age(60 * 60 * 24 * 14) // two weeks
                .path("/");

            return Response::json(jwt);
        }
        catch(const std::exception &e)
        {
            IN_ERR(e.what());
            errors.append("error", "Internal error.");
            return Response::json(errors, HttpStatus::InternalServerError);
        }
        catch(...)
        {
            errors.append("error", "Internal error.");
            return Response::json(errors, HttpStatus::InternalServerError);
        }
    }

    Response Auth::post_create(const crow::request &req)
    {
        auto body = MultipartMap::from(req);

        auto errors = FormErrors();

        // Check honeypot
        if (!body.fields["username2"].empty())
        {
            errors.append("username2", "This field should be empty.");
            return Response::json(errors, HttpStatus::BadRequest);
        }

        // Get fields
        std::string email;
        std::string password;
        try {
            email = body.fields.at("email");
            password = body.fields.at("password");

            if (password != body.fields.at("password2"))
            {
                errors.append("password", "Passwords mismatch.");
                return Response::json(errors, HttpStatus::BadRequest);
            }
        }
        catch (...)
        {
            if (body.fields["email"].empty())
                errors.append("email", "Missing email field.");
            if (body.fields["password"].empty())
                errors.append("password", "Missing password field.");
            return Response::json(errors, HttpStatus::BadRequest);
        }

        // Main validation checks

        if (!std::regex_match(email, Regex::email))
        {
            errors.append("email", "Invalid email address.");
        }

        // Check if user already exists
        auto UserModel = Mongo::Model<User>();
        auto user = UserModel.findOne({"email", email});
        if (user)
        {
            errors.append("email", "User with this email account already "
                "exists.");
        }

        if (password.size() < 3)
        {
            errors.append("password", "Password must be at least 3 characters "
                "long.");
        }

        if (!errors.empty())
        {
            return Response::json(errors, HttpStatus::BadRequest);
        }

        // Create new user
        User newUser;
        newUser.email = email;
        newUser.password = hash(password);
        newUser.type = User::Type::Unverified;

        auto doc = UserModel.insertOne(newUser);
        if (!doc)
        {
            // something went wrong, try again if it was possibly
            // a connection error
            doc = UserModel.insertOne(newUser);
            if (!doc)
            {
                errors.append("error", "Database error.");
                return Response::json(errors, HttpStatus::InternalServerError);
            }
        }

        auto emailStrs = Emails::createVerificationStrings(email,
            doc.value().id.str());

        // Send verification email here
        auto sendEmail = Email::SendEmail();
        auto result = sendEmail
            .to(email)
            .subject("Insound Account Verification")
            .html(emailStrs.html)
            .text(emailStrs.text)
            .send();

        if (!result)
        {
            // try again...
            result = sendEmail.send();
            if (!result)
            {
                return Response::json("Account created, but failed to send verification email");
            }
        }

        return Response::json("Success");
    }

    Response Auth::post_verify(const crow::request &req)
    {
        try {
            auto body = MultipartMap::from(req);
            auto token_it = body.fields.find("token");
            if (token_it == body.fields.end() || token_it->second.empty())
            {
                return Response::json("Missing token.",
                    HttpStatus::BadRequest);
            }

            auto token = Emails::verifyEmail(token_it->second);

            auto UserModel = Mongo::Model<User>();
            auto user = UserModel.findOne({"_id", token.value()._id});

            if (!user || user->body.email != token.value().email)
            {
                return Response::json("Bad token.",
                    HttpStatus::BadRequest);
            }

            if (user->body.isVerified())
            {
                return Response::json("User already verified.");
            }

            user->body.type = User::Type::User;
            auto result = user->save();
            if (!result)
            {
                // try again...
                result = user->save();
                if (!result)
                {
                    return Response::json("Failed to update user validation "
                        "status.", HttpStatus::InternalServerError);
                }
            }

            return Response::json("Success");
        }
        catch(const std::invalid_argument &e)
        {
            return Response::json("Invalid Request.", HttpStatus::BadRequest);
        }

    }

} // namespace Insound
