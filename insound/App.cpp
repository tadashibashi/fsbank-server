#include "App.h"
#include "insound/mongo.h"

#include <insound/routes/api/auth.h>

#include <insound/env.h>
#include <csignal>

namespace Insound {
    static App *app;
    static void gracefulShutdown(int sig)
    {
        switch (sig)
        {
        case SIGTERM:
        case SIGKILL:
        case SIGINT:
        case SIGSTOP:
            IN_LOG("Gracefully shutting down Insound server. Code {}", sig);
            delete app;
            exit(sig);
            break;
        }
    }

    App &App::instance() {
        if (!app)
        {
            app = new App();
            signal(SIGKILL, gracefulShutdown);
            signal(SIGTERM, gracefulShutdown);
            signal(SIGINT, gracefulShutdown);
            signal(SIGSTOP, gracefulShutdown);
        }

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
            // Initialize Catchall
            CROW_CATCHALL_ROUTE(m_app)
            (catch_all);

            // Mount blueprints
            mount<Auth>();

            // Get the PORT from environment
            auto PORT = getEnvType<int>("PORT", 3000);

            // Connect to database
            Mongo::connect();

            m_app.loglevel(crow::LogLevel::Warning);

            try {
                auto result = m_app.port(PORT).multithreaded().run_async();
                std::cout << f("Serving at: http://localhost:{}\n", PORT);
                m_wasInit = true;
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
