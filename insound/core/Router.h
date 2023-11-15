/**
 * Abstract Router base class. Check files in server/routes for examples of
 * implementing this class.
 */
#pragma once
#include <insound/core/thirdparty/crow.hpp>
#include <string_view>

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

        explicit Router(std::string_view route, const RouterOpt &opts = {.useCatchAll=true});
        virtual ~Router() = default;
        crow::Blueprint &config();

      protected:
        crow::Blueprint bp;
        RouterOpt opts;

    private:
        /**
         * Set up blueprint routes here. Must be implemented by child classes.
         */
        virtual void init() = 0;

        /**
         * Route's catchall route. Overridable. (Currently broken)
         */
        virtual void catchAll(const crow::request &req, crow::response &res);
    };
}
