#include "auth.h"
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

    Response Auth::get_check(const crow::request &req)
    {
        Response res;
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

        return res;
    }

    Response Auth::post_login(const crow::request &req)
    {
        Response res;
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
            res.code = 400;
            return res.json("Missing fields.");
        }

        // Check honeypot
        if (!body.fields["password2"].empty())
        {
            res.code = 400;
            errors.append("password2", "Field should be empty.");
            return res.json(errors);
        }

        if (!std::regex_match(email, Regex::email))
        {
            res.code = 400;
            errors.append("email", "Invalid email address.");
            return res.json(errors);
        }

        // Check if user with email exists
        Mongo::Model<User> UserModel;
        auto userRes = UserModel.findOne({"email", email});
        if (!userRes)
        {
            res.code = 401;
            errors.append("email", "Could not find a user with that address.");
            return res.json(errors);
        }

        // Check password
        auto &user = userRes.value();
        if (!compare(password, user.body.password))
        {
            res.code = 401;
            errors.append("password", "Invalid password.");
            return res.json(errors);
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

            res.code = 200;
            return res.json("Success");
        }
        catch(const std::exception &e)
        {
            IN_ERR(e.what());
            res.code = 500;
            errors.append("error", "Internal error.");
            return res.json(errors);
        }
        catch(...)
        {
            res.code = 500;
            errors.append("error", "Internal error.");
            return res.json(errors);
        }
    }

    Response Auth::post_create(const crow::request &req)
    {
        Response res;
        auto body = MultipartMap::from(req);

        auto errors = FormErrors();

        // Check honeypot
        if (!body.fields["username2"].empty())
        {
            res.code = 400;
            errors.append("username2", "This field should be empty.");
            return res.json(errors);
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
                res.code = 400;
                return res.json(errors);
            }
        }
        catch (...)
        {
            res.code = 400;
            if (body.fields["email"].empty())
                errors.append("email", "Missing email field.");
            if (body.fields["password"].empty())
                errors.append("password", "Missing password field.");
            return res.json(errors);
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
            res.code = 400;
            return res.json(errors);
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
                res.code = 500;
                errors.append("error", "Database error.");
                return res.json(errors);
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
                return res.json("Account created, but failed to send verification email");
            }
        }

        return res.json("Success");
    }

    Response Auth::post_verify(const crow::request &req)
    {
        Response res;

        try {
            auto body = MultipartMap::from(req);
            auto token = Emails::verifyEmail(body.fields.at("token"));

            auto UserModel = Mongo::Model<User>();
            auto user = UserModel.findOne({"_id", token.value()._id});

            if (!user || user->body.email != token.value().email)
            {
                res.code = 400;
                return res.json("Invalid token.");
            }

            if (user->body.isVerified())
            {
                return res.json("User already verified.");
            }

            user->body.type = User::Type::User;
            auto result = user->save();
            if (!result)
            {
                // try again...
                result = user->save();
                if (!result)
                {
                    res.code = 500;
                    return res.json("Failed to update user validation status.");
                }
            }

            return res.json("Success");
        }
        catch(const std::invalid_argument &e)
        {
            res.code = 400;
            return res.json("Invalid Request.");
        }

    }

} // namespace Insound
