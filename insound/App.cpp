#include "App.h"
#include "insound/mongo.h"

#include <insound/routes/api/auth.h>

#include <insound/env.h>

namespace Insound {
    static App *app;


    App &App::instance() {
        if (!app)
            app = new App();

        return *app;
    }


    App::App() : m_app(), m_routers(), m_wasInit(false)
    {

    }

    App::~App()
    {
        for (auto router : m_routers)
            delete router;
    }


    std::string catch_all(const crow::request &req)
    {
        IN_LOG("Reached catchall route.");
        return "catchall";
    }


    void App::run()
    {
        if (!m_wasInit) // make sure this function is only called once
        {
            // Configure .env variables
            configureEnv();

            // Mount blueprints
            mount<Auth>();

            // Initialize app
            CROW_CATCHALL_ROUTE(m_app)
            (catch_all);

            // Get the PORT from environment
            auto PORT = getEnvType<int>("PORT", 3000);

            // Connect to database
            Mongo::connect();

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


    void App::mount(Router *router)
    {
        m_routers.emplace_back(router);
        m_app.register_blueprint(router->config());
    }
}
