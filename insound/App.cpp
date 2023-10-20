#include "App.h"

#include <insound/routes/api/auth.h>

#include <insound/env.h>

namespace Insound {
    static App *app;


    App &App::instance() {
        if (!app)
            app = new App();

        return *app;
    }


    App::App() : m_app(), m_bps(), m_wasInit(false)
    {

    }


    std::string catch_all(const crow::request &req)
    {
        IN_LOG("Reached catchall route.");
        auto &user = App::getContext<UserAuth>(req);

        if (user.user.isAuthorized())
            return f("hello {}!", user.user.username);
        else
            return f("hello {}!", "guest");
    }


    void App::run()
    {
        if (!m_wasInit) // make sure this function is only called once
        {
            // Configure .env variables
            configureEnv();

            // Initialize app
            CROW_ROUTE(internal(), "/")
            (catch_all);

            // Mount blueprints
            mount<Auth>();

            // Get the PORT from environment
            auto PORT = getEnvType<int>("PORT", 3000);

            m_app.loglevel(crow::LogLevel::Warning);

            try {
                IN_LOG("Insound server listening at http://localhost:{}", PORT);
                m_wasInit = true;
                m_app.port(PORT).multithreaded().run();
            }
            catch (const std::exception &e)
            {
                IN_ERR("Error during startup: {}", e.what());
            }
        }
    }


    void App::mount(Router &router)
    {
        auto &bp = m_bps.emplace_back(std::move(router.config())) ;
        m_app.register_blueprint(bp);
    }
}
