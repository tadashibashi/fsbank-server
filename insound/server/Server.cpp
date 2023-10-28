#include "Server.h"
#include "insound/core/util.h"

#include <insound/core/definitions.h>
#include <insound/core/mongo.h>
#include <insound/core/s3.h>
#include <insound/server/routes/api/auth.h>

namespace Insound {
    bool Server::init()
    {
        crow::mustache::set_global_base(TEMPLATE_DIR);
        S3::config();
        Mongo::connect();

        mount<Auth>();

        CROW_CATCHALL_ROUTE(this->internal())([](const crow::request &req, crow::response &res) {
            auto page = crow::mustache::load("index.html");

            // set nonce
            auto nonce = genHexString(32);
            res.add_header("Content-Security-Policy",
                f("script-src 'nonce-{}'", nonce));
            crow::mustache::context ctx({{"nonce", nonce}});

            res.code = 200;
            res.body = page.render_string(ctx);
            res.end();
        });

        return true;
    }
}

