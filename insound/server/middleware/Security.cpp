#include "Security.h"
#include <insound/server/Server.h>
#include <insound/core/middleware/Helmet.h>

namespace Insound {

    void Security::after_handle(crow::request &req, crow::response &res,
        context &ctx)
    {
        auto &helmet = Server::getContext<Helmet>(req);
        helmet.headers.contentSecurityPolicy.scriptSrc =
            "'self' 'wasm-unsafe-eval' blob:";
            //f("'nonce-{}' 'wasm-unsafe-eval' blob:", helmet.nonce);
    }

}
