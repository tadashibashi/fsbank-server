#pragma once
#include <insound/core/App.h>
#include <insound/core/Router.h>

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
        static void get_check(const crow::request &req, crow::response &res);

        /**
         * Log into the application
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
        static void post_login(const crow::request &req, crow::response &res);

        /**
         * Create a user account
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
        static void post_create(const crow::request &req, crow::response &res);

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
        static void post_verify(const crow::request &req, crow::response &res);
    };


}
