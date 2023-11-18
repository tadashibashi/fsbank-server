#include "mongo.h"

#include <insound/core/settings.h>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include <cassert>
#include <memory>
#include <utility>

namespace Insound::Mongo
{
    thread_local static std::optional<mongocxx::pool::entry> entry;

    // Singleton class that handles lifetime of mongo objects
    struct AppClient {
        AppClient(): instance{}, pool{}
        { }

        ~AppClient()
        {
        }

        mongocxx::instance instance;
        std::optional<mongocxx::pool> pool;

        bool connect()
        {
            // Already connected
            if (pool) return true;

            try {
                auto uri = mongocxx::uri{Settings::mongoURL()};

                pool.emplace(uri);

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
            pool.reset();
        }

        mongocxx::database db()
        {
            assert(pool);

            if (!entry)
                entry.emplace(pool->acquire());

            return entry.value()->database(Settings::mongoDBName());
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
