#pragma once
#include <insound/core/thirdparty/crow.hpp>
#include <string>

namespace crow {
    class Blueprint;
}

namespace Insound {
    class Router
    {
    public:
        struct RouterOpt {
            bool useCatchAll;
        };

        explicit Router(const std::string &route, const RouterOpt &opts = {.useCatchAll=true});
        virtual ~Router() = default;

        crow::Blueprint &config();

      protected:
        crow::Blueprint bp;
        RouterOpt opts;

    private:
        virtual void init() = 0;

        /**
         * Route's catchall route. Overridable.
         */
        virtual void catchAll(const crow::request &req, crow::response &res);
    };
}
