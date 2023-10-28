#include "Server.h"
#include <insound/core/s3.h>
#include <insound/server/routes/api/auth.h>
#include <insound/core/definitions.h>

namespace Insound {
    bool Server::init() {
        crow::mustache::set_global_base(TEMPLATE_DIR);

        mount<Auth>();

        S3::config();

        return true;
    }
}

