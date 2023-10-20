#pragma once
#include <crow.h>
#include <insound/app.h>

namespace Insound::Auth {
    crow:: Blueprint &config();

    crow::response post_login(const crow::request &req);
}
