#include "Server.h"

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

        CROW_CATCHALL_ROUTE(this->internal())([]() {
            return crow::mustache::load("index.html");
        });

        return true;
    }
}

