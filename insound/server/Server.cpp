#include "Server.h"
#include <insound/core/s3.h>
#include <insound/server/routes/api/auth.h>

namespace Insound {
    bool Server::init() {
        mount<Auth>();

        S3::config();

        return true;
    }
}

