#include "server.h"
#include <crow/routing.h>
#include "controllers/api/auth.h"

namespace Insound {
    static crow::Crow<> app;
    static bool wasInit;

    void initApp()
    {
        if (!wasInit)
        {
            // Initialize app
            CROW_ROUTE(app, "/")([]() {
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
