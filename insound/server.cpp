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
            const int DefaultPort = 1234;
            int PORT;
            if (auto portEnv = std::getenv("PORT"))
            {
                try {
                    PORT = std::atoi(portEnv);
                } catch (...) {
                    std::cerr << "Invalid PORT: not a number. Reverting to default " <<
                        DefaultPort << '\n';
                    PORT = DefaultPort;
                }
            }
            else
            {
                PORT = DefaultPort;
            }

            app.register_blueprint(auth);
            app.loglevel(crow::LogLevel::Warning);

            std::cout << "Insound server listening at http://localhost:" << PORT << '\n';
            wasInit = true;
            app.port(PORT).multithreaded().run();
        }
    }
}
