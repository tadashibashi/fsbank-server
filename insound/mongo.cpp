#include "mongo.h"

#include <insound/env.h>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include <memory>
#include <utility>

namespace Insound::Mongo {
    thread_local static std::optional<mongocxx::pool::entry> entry;
    static std::string MONGO_DBNAME;

    // Singleton class that handles lifetime of mongo objects
    struct AppClient {
        AppClient(): instance{}, pool{}
        { }

        ~AppClient()
        {
            delete pool;
        }

        mongocxx::instance instance;
        mongocxx::pool *pool;

        bool connect()
        {
            // Already connected
            if (pool) return true;

            try {
                if (MONGO_DBNAME.empty())
                    MONGO_DBNAME = requireEnv("MONGO_DBNAME");
                auto uri = mongocxx::uri{requireEnv("MONGO_URL")};
                if (!pool)
                {
                    pool = new mongocxx::pool{uri};
                }
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

        void disconnect()
        {
            delete pool;
            pool = nullptr;
        }

        mongocxx::database db()
        {
            if (!entry)
                entry.emplace(pool->acquire());

            return entry.value()->database(MONGO_DBNAME);
        }
    };

    static AppClient sClient{};

    bool connect()
    {
        return sClient.connect();
    }

    mongocxx::database db()
    {
        return sClient.db();
    }
}
