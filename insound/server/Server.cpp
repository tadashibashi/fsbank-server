#include "Server.h"
#include <insound/server/routes/api/auth.h>

namespace Insound {
    bool Server::init() {
        mount<Insound::Auth>();

        return true;
    }
}

