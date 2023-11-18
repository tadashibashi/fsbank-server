/**
 * @file App.h
 *
 * App is the base class for an http server that other projects should inherit
 * from.
 *
 * Please see server/Server.h for an example of how to implement this class.
 */
#pragma once
#include <insound/core/env.h>
#include <insound/core/mongo.h>
#include <insound/core/Router.h>
#include <insound/core/middleware/Helmet.h>
#include <insound/core/middleware/UserAuth.h>

#include <insound/core/thirdparty/crow.hpp>
#include <crow/middlewares/cookie_parser.h>

#include <memory>
#include <type_traits>
#include <vector>

namespace Insound
{
    using LogLevel = crow::LogLevel;

    /**
     * Options to set when initializing an `App`.
     */
    struct AppOpts
    {
        // Default port number if var "PORT" is not provided to the environment
        int defaultPort = 3000;

        // Log level of the application. E.g. `LogLevel::Warning` will only log
        // warnings and anything more severe.
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
            m_opts(opts)
        { }

        /**
         * Retrieve App singleton
         */
        static App &instance()
        {
            return s_instance ? *s_instance :
                *(s_instance = std::make_shared<App<Middlewares...>>());
        }

        virtual ~App() = default;

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
            mount(std::make_unique<T>(std::forward<TArgs>(args)...));
        }

        /**
         * Get middleware context object from the singleton App
         *
         * @param req - current route's request object
         */
        template <typename MiddlewareType>
        static typename MiddlewareType::context &getContext(
            const crow::request &req)
        {
            return instance().m_app.template get_context<MiddlewareType>(req);
        }

        /**
         * Start the server
         *
         * @return boolean whether app is now running - false on error, or if
         *                 app was already running - true on success.
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
                return false;
            }
            catch(...)
            {
                IN_ERR("An exception occurred during App<...>::run()");
                return false;
            }
        }

        /**
         * Mount a router onto the App
         *
         * @param  router - the router - ownership is passed to the App so it
         *                  must be moved
         *
         * @return app object for chaining.
         */
        App &mount(std::unique_ptr<Router> router)
        {
            try {
                m_app.register_blueprint(router->config());
            }
            catch (const std::runtime_error &e)
            {
                IN_WARN("Called `App::mount`, but router was already mounted");
                return *this;
            }
            catch(...)
            {
                IN_ERR("Uknown error occurred during `App::mount` router");
                throw;
            }

            m_routers.emplace_back(std::move(router));

            return *this;
        }

        /**
         * Get internal crow application
         */
        [[nodiscard]]
        Server &internal() { return m_app; }

        /**
         * Get whether app was already initialized via a successful call to
         * `App::run`
         */
        [[nodiscard]]
        bool wasInit() const { return m_wasInit; }
    private:
        /**
         * Contains initialization logic for the app. Sub-classes should
         * implement this virtual function for its own initialization
         *
         * @see insound/server/Server.h for an example of implementation
         *
         * @return whether initialization was successful.
         */
        virtual bool init() { return true; };

        bool m_wasInit;

        Server m_app;
        std::vector<std::unique_ptr<Router>> m_routers;
        AppOpts m_opts;
    };
}
