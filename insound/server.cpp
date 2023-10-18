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
            // Configure .env variables
            configureEnv();

            // Initialize app
            CROW_ROUTE(app, "/")([](const crow::request &req) {
                auto &userCtx = app.get_context<UserContext>(req);

                return "Hello from Insound Server!";
            });

            // Mount blueprints
            auto auth = Auth::config();
            app.register_blueprint(auth);

            // Get the PORT from environment
            auto PORT = getEnvType<int>("PORT", 3000);

            app.loglevel(crow::LogLevel::Info);



            try {
                auto result = app.port(PORT).multithreaded().run_async();
                std::cout << "Insound server listening at http://localhost:" << PORT << '\n';
            } catch (const std::exception &e)
            {
                std::cerr << "Error during startup: " << e.what() << '\n';
            }

            wasInit = true;
        }
    }
}
