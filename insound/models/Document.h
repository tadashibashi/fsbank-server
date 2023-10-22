#pragma once

#include <insound/errors/GlazeError.h>
#include <insound/mongo.h>
#include <insound/thirdparty/glaze.hpp>

#include <bsoncxx/builder/list.hpp>
#include <bsoncxx/json.hpp>

#include <glaze/core/common.hpp>
#include <glaze/core/context.hpp>
#include <glaze/util/expected.hpp>

#include <string>
#include <string_view>

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
        static void ensureCollectionExists(std::string_view name) {
            if (!db().has_collection(name))
                throw std::runtime_error(f("Mongo::Document error: "
                    "Collection {} doesn't exist in the database",
                    name));
        }
    public:
        explicit Document(std::string_view collection) : id(), body(),
            cName(collection) {
                ensureCollectionExists(collection);
        }

        Document(std::string_view collection, const T &body) : id(),
            body(body), cName(collection)
        {
            ensureCollectionExists(collection);
        }

        static Document<T> fromBson(std::string_view name,
            const bsoncxx::document::view_or_value &bson)
        {
            T obj;
            auto json = bsoncxx::to_json(bson.view());
            auto err = glz::read<glz::opts{
                .error_on_unknown_keys=false,
                .error_on_missing_keys=true
            }>(obj, json);

            if (err)
                throw GlazeError(err, json);
            
            auto doc = Document<T>(name, obj);
            doc.id = bson.view()["_id"].get_oid().value.to_string();
            return doc;
        }

        bool save()
        {
            auto collection = db().collection(cName);

            auto json = glz::write_json(body);
            auto bson = bsoncxx::from_json(json);

            if (id.empty()) // new document, generate new id
            {
                auto result = collection.insert_one(bson.view());
                if (result && result.value().result().inserted_count())
                {
                    id = result.value().inserted_id().get_oid().value.to_string();
                    return true;
                }
                return false;
            }
            else
            {
                auto opts = mongocxx::options::find_one_and_update();
                opts.upsert(true);
                auto query = bsoncxx::builder::list({"_id", id});
                auto result = collection.find_one_and_update(
                   query.view().get_document().value, bson.view(), opts);
                return static_cast<bool>(result);
            }
        }

        std::string id;
        T body;

    private:
        std::string_view cName;
    };
}

template<>
struct glz::meta<Insound::Mongo::Id> {
    using T = Insound::Mongo::Id;
    static constexpr auto value = glz::object(
        "$oid", &T::id
    );
};
