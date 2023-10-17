#include "server.h"
#include "controllers/auth.h"

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

            CROW_ROUTE(app, "/api/auth/login")
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


            app.port(PORT).multithreaded().run();
            wasInit = true;
        }
    }
}
