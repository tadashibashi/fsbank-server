#include "Router.h"
#include "crow/app.h"

using crow::response;
using crow::request;

namespace Insound {

    Router::Router(const std::string &route, const RouterOpt &opts) :
        bp(route), opts(opts)
    { }

    crow::Blueprint &Router::config()
    {
        init();

        if (opts.useCatchAll)
        {
            CROW_BP_CATCHALL_ROUTE(bp)(
            [this](const request &req, response &res) {
                this->catchAll(req, res);
            });
        }

        return bp;
    }

    void Router::catchAll(const crow::request &req, crow::response &res)
    {
        IN_LOG("Hit bp catchall route");
        res.end("404 Not found.");
    }
}
