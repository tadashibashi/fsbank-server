#include "server.h"

namespace Insound {
    static crow::Crow<> app;
    static bool wasInit;

    crow::Crow<> &getApp()
    {
        if (!wasInit)
        {
            // Initialize app
            CROW_ROUTE(app, "/")([]() {
                return "Hello Crow App";
            });

            wasInit = true;
        }

        return app;
    }
}
