#include "Router.h"
#include "crow/app.h"

crow::Blueprint &Insound::Router::config()
{
    configImpl();

    if (opts.useCatchAll)
    {
        // bp.catchall_rule()(
        // [this](const crow::request &req, crow::response &res) {
        //     this->catchAll(req, res);
        // });
    }

    IN_LOG("BP has catchall route: {}", bp.catchall_rule().has_handler());

    return bp;
}

void Insound::Router::catchAll(const crow::request &req, crow::response &res)
{
    IN_LOG("Hit bp catchall route");
    res.end("404 Not found.");
}
