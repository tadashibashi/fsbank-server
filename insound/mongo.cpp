#include "mongo.h"

#include <insound/env.h>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

namespace Insound::Mongo {
    static mongocxx::instance sInstance{};
    static mongocxx::client sClient;

    static mongocxx::database sDB;

    mongocxx::database &db() {
        return sDB;
    }

    bool connect()
    {
        try {
            sClient = mongocxx::client(
            mongocxx::uri(requireEnv("MONGO_URL")));
            sDB = sClient[requireEnv("MONGO_DBNAME")];

            auto users = sDB.collection("users");
            auto user = users.find_one({});
            if (user.has_value())
                IN_LOG(bsoncxx::to_json(user.value()));

            return true;
        }
        catch (const std::exception &e)
        {
            IN_ERR("Error during db init: {}", e.what());
            return false;
        }
        catch (...)
        {
            IN_ERR("Error occurred during db init.");
            return false;
        }
    }
}
