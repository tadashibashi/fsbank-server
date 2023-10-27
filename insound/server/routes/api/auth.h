#pragma once
#include <insound/core/App.h>

#include <insound/core/Router.h>

namespace Insound {
    class Auth : public Router {
    public:
        Auth();

        void init();

        static void get_check(const crow::request &req, crow::response &res);
        static void post_login(const crow::request &req, crow::response &res);
        static void post_create(const crow::request &req, crow::response &res);
    };


}
