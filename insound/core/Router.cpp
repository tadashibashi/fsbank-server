#include "Router.h"
#include "crow/app.h"

namespace Insound {

    Router::Router(std::string_view route, const RouterOpt &opts) :
        bp(route.data()), opts(opts)
    { }

    crow::Blueprint &Router::config()
    {
        init();

        if (opts.useCatchAll)
        {
            // This feature is currently broken in Crow
            // CROW_BP_CATCHALL_ROUTE(bp)(
            // [this](const crow::request &req, crow::response &res) {
            //     this->catchAll(req, res);
            // });
        }

        return bp;
    }

    void Router::catchAll(const crow::request &req, crow::response &res)
    {
        IN_LOG("Hit bp catchall route");
        res.end("404 Not found.");
    }
}
