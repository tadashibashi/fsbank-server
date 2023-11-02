#pragma once
#include "Id.h"

#include <insound/core/errors/GlazeError.h>
#include <insound/core/mongo.h>

#include <insound/core/thirdparty/glaze.hpp>

#include <bsoncxx/builder/list.hpp>
#include <bsoncxx/json.hpp>

#include <glaze/core/common.hpp>
#include <glaze/core/context.hpp>
#include <glaze/util/expected.hpp>

#include <string>
#include <string_view>

namespace Insound::Mongo {
    /**
     * @tname T - must be registered with glz::meta
     */
    template <typename T>
    class Document
    {
    private:
        static void assertCollectionExists(std::string_view name) {
            if (!db().has_collection(name))
                throw std::runtime_error(f("Mongo::Document error: "
                    "Collection ({}) doesn't exist in the database",
                    name));
        }
    public:
        Document() : id(), body()
        {
                assertCollectionExists(glz::meta<T>::name);
        }

        explicit Document(const T &body) : id(), body(body)
        {
            assertCollectionExists(glz::meta<T>::name);
        }

        /**
         * Convert a raw bson document from MongoDB to this Document<T>
         */
        static Document<T> fromBson(
            const bsoncxx::document::view_or_value &bson)
        {
            T obj;
            auto json = bsoncxx::to_json(bson.view());
            auto err = glz::read<glz::opts{
                .error_on_unknown_keys=false,
                .error_on_missing_keys=false
            }>(obj, json);

            if (err)
                throw GlazeError(err, json);
            
            auto doc = Document(obj);
            doc.id = Id::fromBsonDocument(bson);
            return doc;
        }

        bool save()
        {
            auto collection = db().collection(glz::meta<T>::name);

            auto json = glz::write_json(body);
            auto bson = bsoncxx::from_json(json);

            if (!id) // new document, generate new id
            {
                auto result = collection.insert_one(bson.view());
                if (result && result.value().result().inserted_count())
                {
                    id = Id{result.value().inserted_id().get_oid().value};
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

        Id id;
        T body;
    };
}
