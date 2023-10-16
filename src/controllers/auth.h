#pragma once
#include <crow.h>

namespace Insound::Auth {
    crow::response post_login(const crow::request &req);
}
