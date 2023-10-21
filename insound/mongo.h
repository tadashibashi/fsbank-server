#pragma once
#include <mongocxx/database.hpp>

namespace Insound::Mongo
{
    bool connect();

    mongocxx::database &db();
}
