#include "Server.h"

#include <insound/core/definitions.h>
#include <insound/core/email.h>
#include <insound/core/env.h>
#include <insound/core/mongo.h>
#include <insound/core/s3.h>
#include <insound/core/util.h>
#include <insound/server/routes/api/auth.h>

#include <insound/core/middleware/Helmet.h>

namespace Insound {

    static void catchAll(const crow::request &req, crow::response &res) {
        auto &helmet = Server::getContext<Helmet>(req);

        crow::mustache::set_base(TEMPLATE_DIR "/");
        auto page = crow::mustache::load("index.html");

        crow::mustache::context ctx({{"nonce", helmet.nonce}});

        res.end(page.render_string(ctx));
    }

    bool Server::init()
    {
        configureEnv();

        bool result;
        result = S3::config();
        if (result)
            IN_LOG("S3 client initialized, connected to: {}",
                requireEnv("AWS_ENDPOINT_URL"));
        else
            IN_ERR("S3 client failed to initialize.");

        result = Mongo::connect();
        if (result)
            IN_LOG("MongoDB client initialized, connected to: {}",
                requireEnv("MONGO_URL"));
        else
            IN_ERR("MongoDB client failed to connect.");

        result = Email::config();
        if (result)
            IN_LOG("Email API configured.");
        else
            IN_ERR("Email failed to configure.");

        mount<Auth>();



        CROW_ROUTE(this->internal(), "/<path>")([](const crow::request &req,
            crow::response &res, const std::string &path)
        {
            catchAll(req, res);
        });
        CROW_ROUTE(this->internal(), "/")(catchAll);

        return true;
    }
}

