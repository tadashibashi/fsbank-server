#include "server.h"
#include <crow.h>
#include "controllers/api/auth.h"
#include "insound/env.h"
#include "middleware/UserContext.hpp"

namespace Insound {
    static crow::Crow<UserContext> app;
    static bool wasInit;

    void initApp()
    {
        if (!wasInit)
        {
            configureEnv();

            // Initialize app
            CROW_ROUTE(app, "/")([](const crow::request &req) {
                auto &userCtx = app.get_context<UserContext>(req);

                return "Hello from Insound Server!";
            });

            crow::Blueprint auth("api/auth");

            CROW_BP_ROUTE(auth, "/login/email")
                .methods(crow::HTTPMethod::POST)
                (Auth::post_login);

            // Get the PORT from environment
            auto PORT = getEnv<int>("PORT", 3000);

            app.register_blueprint(auth);
            app.loglevel(crow::LogLevel::Warning);

            std::cout << "Insound server listening at http://localhost:" << PORT << '\n';
            wasInit = true;
            app.port(PORT).multithreaded().run();
        }
    }
}
