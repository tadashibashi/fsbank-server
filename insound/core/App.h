#pragma once
#include <insound/core/env.h>
#include <insound/core/mongo.h>
#include <insound/core/Router.h>
#include <insound/core/middleware/Helmet.h>
#include <insound/core/middleware/UserAuth.h>

#include <insound/core/thirdparty/crow.hpp>
#include <crow/middlewares/cookie_parser.h>

#include <type_traits>
#include <vector>

namespace Insound {
    using LogLevel = crow::LogLevel;

    struct AppOpts {
        int defaultPort = 3000;
        LogLevel logLevel = LogLevel::Warning;
    };

    /**
     * @tparam Middlewares - a list of additional user-provided middleware
     */
    template <typename ...Middlewares>
    class App
    {
        inline static std::shared_ptr<App<Middlewares...>> s_instance = nullptr;
    public:
        using Server = crow::App<Helmet, crow::CookieParser, UserAuth,
            Middlewares...>;

        App(const AppOpts &opts = {}): m_app(), m_routers(), m_wasInit(),
            m_opts(opts) {

        }

        /**
         * Retrieve App singleton
         */
        static App &instance()
        {
            return s_instance ? *s_instance :
                *(s_instance = std::make_shared<App<Middlewares...>>());
        }

        ~App() { for (auto router : m_routers) delete router; }

        /**
         * Mount a router to the App.
         * @tparam T     - type of the router - it must extend
         *                 `Insound::Router`
         * @tparam TArgs - variadic parameter pack typing args passed to
         *                 type `T` Router's constructor
         * @param args - args to pass to the `T` Router's constructor
         * @example
         *     `app.mount<T>();`
         *
         */
        template <typename T, typename ...TArgs>
            requires std::is_base_of_v<Router, T>
        void mount(TArgs &&...args)
        {
            mount(new T(std::forward<TArgs>(args)...));
        }

        /**
         * Get middleware context object from the singleton App.
         * @param req - current route's request object
         */
        template <typename MiddlewareType>
        static typename MiddlewareType::context &getContext(const crow::request &req)
        {
            return instance().m_app.template get_context<MiddlewareType>(req);
        }

        /**
         * Start the server
         */
        bool run()
        {
            if (m_wasInit)
            {
                IN_WARN("App::run was called, but it's already initialized.");
                return false;
            }

            try
            {
                // Run child class init function
                if (!init()) return false;

                // Singleton services
                Mongo::connect();

                auto PORT = getEnv<int>("PORT", m_opts.defaultPort);

                m_app
                    .server_name("insound")
                    .loglevel(m_opts.logLevel)
                    .port(PORT)
                    .multithreaded()
                    .run_async();

                return m_wasInit = true;
            }
            catch(const std::exception &e)
            {
                IN_ERR("An exception occurred during App<...>::run(): {}",
                    e.what());
                throw;
            }
            catch(...)
            {
                IN_ERR("An exception occurred during App<...>::run()");
                throw;
            }

            return false;
        }

        /**
         * Mount a router onto the App
         * @param  router [description]
         * @return        [description]
         */
        App &mount(Router *router)
        {
            try
            {
                m_app.register_blueprint(router->config());
            }
            catch (const std::runtime_error &e)
            {
                IN_WARN("Called App::mount, but router was already mounted");
                return *this;
            }
            catch(...)
            {
                IN_ERR("Uknown error occurred during App::mount router");
                throw;
            }

            m_routers.emplace_back(router);

            return *this;
        }

        /**
         * Get internal crow application
         */
        Server &internal() { return m_app; }

    private:
        virtual bool init() { return true; };

        bool m_wasInit;

        Server m_app;
        std::vector<Router *> m_routers;
        AppOpts m_opts;
    };
}
