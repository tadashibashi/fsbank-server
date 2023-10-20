#include "app.h"

#include <insound/routes/api/auth.h>

#include <insound/env.h>
#include <insound/log.h>

namespace Insound {
    static App app;
    static bool wasInit;

    App &getApp() {
        return app;
    }

    void initApp()
    {
        if (!wasInit) // make sure this function is only called once
        {
            // Configure .env variables
            configureEnv();

            // Initialize app
            CROW_CATCHALL_ROUTE(app)([]() {
                //auto &userCtx = getContext<UserAuth>(req);
                return "hello insound!";
            });

            // Mount blueprints
            app.register_blueprint(Auth::config());

            // Get the PORT from environment
            auto PORT = getEnvType<int>("PORT", 3000);

            app.loglevel(crow::LogLevel::Info);



            try {
                std::cout << "Insound server listening at http://localhost:" << PORT << '\n';
                wasInit = true;
                app.port(PORT).multithreaded().run();
            } catch (const std::exception &e)
            {
                IN_LOG("Error during startup: {}", e.what());
            }
        }
    }
}
