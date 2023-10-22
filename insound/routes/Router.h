#pragma once
#include <insound/thirdparty/crow.hpp>

namespace Insound {
    class Router
    {
    public:
        struct RouterOpt {
            bool useCatchAll;
        };
        explicit Router(const std::string &route, RouterOpt opts = {.useCatchAll=true}) : bp(route),
            opts(opts) { }
        
        Router(Router &&router) : bp(std::move(router.bp)),
            opts(router.opts)
        {}

        virtual ~Router() = default;

        crow::Blueprint &config();



      protected:
        crow::Blueprint bp;
        RouterOpt opts;

    private:
        virtual void configImpl() = 0;

        /**
         * Route's catchall route. Overridable.
         */
        virtual void catchAll(const crow::request &req, crow::response &res);
    };
}
