#pragma once
#include <insound/routes/Router.h>
#include <insound/thirdparty/crow.hpp>
#include <crow/middlewares/cookie_parser.h>
#include <insound/middleware/UserAuth.hpp>
#include <type_traits>
#include <vector>

namespace Insound {
    using CrowApp = crow::App<crow::CookieParser, UserAuth>;

    class App
    {
    public:
        /**
         * Retrieve App singleton
         */
        static App &instance();
        ~App();

        /**
         * Mount a router to the App.
         * Forwards arguments to `T` router's constructor.
         *
         * @example
         *     `app.mount<T>();`
         *
         */
        template <typename T, typename ...TArgs> requires std::is_base_of_v<Router, T>
        void mount(TArgs &&...args)
        {
            mount(new T(std::forward<TArgs>(args)...));
        }

        template <typename MiddlewareType>
        static typename MiddlewareType::context &getContext(const crow::request &req)
        {
            return instance().m_app.get_context<MiddlewareType>(req);
        }

        void run();

        /**
         * Get internal crow application
         */
        CrowApp &internal() { return m_app; }

    private:
        App();
        void mount(Router *router);

        bool m_wasInit;
        CrowApp m_app;
        std::vector<Router *> m_routers;
    };
}
