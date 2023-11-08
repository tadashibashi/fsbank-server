#pragma once
#include <insound/core/App.h>
#include <insound/core/Router.h>
#include <insound/core/Response.h>

namespace Insound {
    class Auth : public Router {
    public:
        Auth();

        void init() override;

        /**
         * @route GET /api/auth/check
         *
         * Check whether User is authenticated / logged in.
         * Client should check in via this route to ensure its token is ok.
         *
         * @input
         * Headers:
         * {
         *     Authorization: Bearer <json-web-token>
         * }
         *
         * @return
         * JSON:
         * {
         *     auth: boolean
         * }
         *
         */
        static Response check(const crow::request &req);

        /**
         * Log into the application via email. Other types of login may be
         * supported at a later time.
         *
         * @route POST /api/auth/login/email
         * @input
         * form-data:
         * {
         *     email: string,
         *     password: string
         * }
         *
         * `password2` is a honeypot to help prevent bots
         */
        static Response login_email(const crow::request &req);

        /**
         * Create a user account via email
         *
         * @route POST /api/auth/create/email
         * @input
         * form-data:
         * {
         *     email: string,
         *     password: string,
         *     password2: string,
         * }
         *
         * `password` must match `password2`
         *
         * `username2` is a honeypot to help prevent bots
         */
        static Response create_email(const crow::request &req);

        /**
         * Email verification endpoint for automated emails
         *
         * @route POST /api/auth/verify
         *
         * @input
         * form-data:
         * {
         *     token: string
         * }
         *
         * `token` is the validation token received in the email
         */
        static Response activate(const crow::request &req);
    };


}
