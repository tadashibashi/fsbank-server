#include "Server.h"

#include "insound/core/util.h"

#include <insound/core/definitions.h>
#include <insound/core/mongo.h>
#include <insound/core/s3.h>
#include <insound/server/routes/api/auth.h>

namespace Insound {
    bool Server::init()
    {
        bool result;
        result = S3::config();
        if (result)
            IN_LOG("S3 connected at: {}", requireEnv("AWS_ENDPOINT_URL"));
        else
            IN_ERR("S3 failed to connect");

        result = Mongo::connect();
        if (result)
            IN_LOG("Mongo DB connected at: {}", requireEnv("MONGO_URL"));

        mount<Auth>();

        CROW_CATCHALL_ROUTE(this->internal())(
            [](const crow::request &req, crow::response &res) {
                crow::mustache::set_base(TEMPLATE_DIR "/");
                auto page = crow::mustache::load("index.html");

                // set nonce
                auto nonce = genBytes(16);
                auto nonceStr = crow::utility::base64encode(nonce.data(), nonce.size());
                res.add_header("Content-Security-Policy",
                    f("script-src 'nonce-{0}';style-src 'nonce-{0}'", nonceStr));
                crow::mustache::context ctx({{"nonce", nonceStr}});

                res.code = 200;
                res.body = page.render_string(ctx);
                res.end();
            });

        return true;
    }
}

