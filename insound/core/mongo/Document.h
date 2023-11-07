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
            assertCollectionExists(glz::meta<T>::collection_name);
        }

        explicit Document(const T &body) : id(), body(body)
        {
            assertCollectionExists(glz::meta<T>::collection_name);
        }

        /**
         * Convert a raw bson document retrived from MongoDB to this Document.
         * This is mainly used Mongo::Model, and most likely does not need
         * to be used directly by the end user.
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


        /**
         * Save this Document to its collection. Adds a new document if this
         * is a new instance, or updates a db-retrieved document that was
         * already once created.
         *
         * @return whether save was successful
         */
        bool save()
        {
            auto collection = db().collection(glz::meta<T>::collection_name);

            auto json = glz::write_json(body);
            auto bson = bsoncxx::from_json(json);

            if (!id) // this is a new document
            {
                // Insert doc
                auto result = collection.insert_one(bson.view());
                if (result && result.value().result().inserted_count())
                {
                    // Insert success: get the generated id and update local id
                    id = Id{result.value().inserted_id().get_oid().value};
                    return true;
                }

                // Insert failed
                return false;
            }
            else   // this is a previously created document, or a new one where
                   // the user manually set the id
            {
                // Set upsert to true. Covers the case where the user manually
                // sets the Document's id, but it does not exist in the
                // database yet.
                auto opts = mongocxx::options::find_one_and_update();
                opts.upsert(true);

                // Find doc and update it
                auto query = bsoncxx::builder::list({"_id", id});
                auto result = collection.find_one_and_update(
                   query.view().get_document().value, bson.view(), opts);

                // Any returned doc means the op succeeded.
                return static_cast<bool>(result);
            }
        }

        Id id;
        T body;
    };
}
