#pragma once
#include <insound/routes/Router.h>
#include <insound/thirdparty/crow.hpp>
#include <insound/middleware/UserAuth.hpp>
#include <type_traits>
#include <vector>

namespace Insound {
    using CrowApp = crow::App<UserAuth>;

    class App
    {
    public:
        /**
         * Retrieve App singleton
         */
        static App &instance();
        ~App();

        /**
         * Mount a router to the App
         *
         * @example
         *     `app.mount<RouterType>();`
         *
         */
        template <typename T> requires std::is_base_of_v<Router, T>
        void mount()
        {
            auto t = T();
            mount(t);
        }

        template <typename MiddlewareType>
            requires std::is_base_of_v<crow::ILocalMiddleware, MiddlewareType>
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
        void mount(Router &router);

        bool m_wasInit;
        CrowApp m_app;
        std::vector<crow::Blueprint> m_bps;
    };
}
