#include "Server.h"
#include "insound/core/s3.h"
#include <insound/server/routes/api/auth.h>

namespace Insound {
    bool Server::init() {
        mount<Insound::Auth>();

        S3::config();
        auto files = S3::listObjects();
        IN_LOG("File size: {}", files.size());
        return true;
    }
}

