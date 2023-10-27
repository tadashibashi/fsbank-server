#pragma once
#include <insound/core/App.h>

#include <insound/core/Router.h>

namespace Insound {
    class Auth : public Router {
    public:
        Auth();

        void init();

        static crow::response get_check(const crow::request &req);
        static crow::response &post_login(const crow::request &req, crow::response &res);
        static crow::response &post_create(const crow::request &req, crow::response &res);
    };


}
