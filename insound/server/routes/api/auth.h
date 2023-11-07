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
        static Response get_check(const crow::request &req);

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
        static Response post_login(const crow::request &req);

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
        static Response post_create(const crow::request &req);

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
        static Response post_verify(const crow::request &req);
    };


}
