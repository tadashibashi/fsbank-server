#pragma once

#include <insound/errors/GlazeError.h>
#include <insound/mongo.h>
#include <insound/thirdparty/glaze.hpp>

#include <bsoncxx/json.hpp>

#include <glaze/core/common.hpp>
#include <glaze/core/context.hpp>
#include <glaze/util/expected.hpp>

#include <string>

namespace Insound::Mongo {
    struct Id {
        std::string id;

        // Should automatically resolve to its internal id
        // when needed
        operator std::string() { return id; }
    };

    /**
     * @tname T - must be registered with glz::meta
     */
    template <typename T>
    class Document
    {
    private:
        static void ensureCollectionExists(const std::string &name) {
            if (!db().has_collection(name))
                throw std::runtime_error(f("Mongo::Document error: "
                    "Collection {} doesn't exist in the database",
                    name));
        }
    public:
        explicit Document(const std::string &collection) : id(), body(),
            cName(collection) {
                ensureCollectionExists(collection);
        }

        Document(const std::string &collection, const T &body) : id(),
            body(body), cName(collection)
        {
            ensureCollectionExists(collection);
        }

        static Document fromBson(const std::string &name,
            const bsoncxx::document::view_or_value &bson)
        {
            Document doc;
            auto json = bsoncxx::to_json(bson);
            auto err = glz::read<glz::opts{
                .error_on_unknown_keys=false,
                .error_on_missing_keys=true
            }>(doc.body, json);

            if (err)
                throw GlazeError(err, json);

            return doc;
        }

        bool save()
        {
            auto collection = db().collection(cName);

            auto json = glz::write_json(body);
            auto bson = bsoncxx::from_json(json);

            if (id.empty()) // new document, generate new id
            {
                auto newId = bsoncxx::oid{};
                bson["_id"] = newId;
                auto result = collection.insert_one(bson);
                if (!result) return false;

                id = newId.to_string();
                return true;
            }
            else
            {
                bson["_id"] = bsoncxx::oid{id};
                auto opts = mongocxx::options::find_one_and_update();
                opts.upsert(true);
                auto result = collection.find_one_and_update(
                    {"_id", bson["_id"]}, bson, opts);
                return static_cast<bool>(result);
            }
        }

        std::string id;
        T body;

    private:
        std::string cName;
    };
}

template<>
struct glz::meta<Insound::Mongo::Id> {
    using T = Insound::Mongo::Id;
    static constexpr auto value = glz::object(
        "$oid", &T::id
    );
};
