#include "server.h"
#include "controllers/auth.h"

namespace Insound {
    static crow::Crow<> app;
    static bool wasInit;

    crow::Crow<> &getApp()
    {
        if (!wasInit)
        {
            // Initialize app
            CROW_ROUTE(app, "/")([]() {
                return "Hello from Insound Server!";
            });

            CROW_ROUTE(app, "/api/auth/login")
                .methods(crow::HTTPMethod::POST)
                (Auth::post_login);

            wasInit = true;
        }

        return app;
    }
}
