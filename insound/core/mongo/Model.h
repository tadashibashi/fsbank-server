#pragma once

#include <insound/core/mongo/Document.h>
#include <bsoncxx/builder/list.hpp>
#include <mongocxx/collection.hpp>

#include <optional>
#include <string>
#include <vector>

namespace Insound::Mongo {

    /**
     * A Model that manages a collection
     * @tname Schema - a class or struct that has specialized glz::meta
     */
    template <typename Schema>
    class Model {
    public:
        using bson = bsoncxx::builder::list;

        /**
         * Create the model with collection name
         */
        explicit Model() :
            m_collection()
        {
            auto name = glz::meta<Schema>::name;
            if (!Mongo::db().has_collection(name))
                m_collection = Mongo::db().create_collection(name);
            else
                m_collection = Mongo::db().collection(name);
        }


        /**
         * Insert a document into the database - it is created by
         * passing in an instance of the type of schema this Model represents
         * (schemas are a struct that specializes the glz::meta class)
         *
         * @param obj - the schema object instance to use to create the doc
         *
         * @return the document, or none if there was a problem saving it to
         *         the database.
         */
        std::optional<Document<Schema>> insertOne(const Schema &obj)
        {
            auto doc = Document(obj);
            if (doc.save())
                return doc;
            return {};
        }


        /**
         * Find a document that matches the bson query.
         *
         * @param query - bson document that can be built like so:
         *                `{"email", "joe@joe.com"}`
         *
         * This query would search for one document with an email field that
         * matches "joe@joe.com".
         *
         * @return Document object if found, or none if one does not exist.
         */
        [[nodiscard]]
        std::optional<Document<Schema>> findOne(bson query)
        {
            auto queryDoc = query.view().get_document().value;
            auto doc = m_collection.find_one(queryDoc);
            if (!doc) return {};

            return Document<Schema>::fromBson(doc.value().view());
        }

        [[nodiscard]]
        std::optional<Document<Schema>> findById(const Id id)
        {
            if (!id.oid()) return {}; // empty id provided

            auto queryDoc = bsoncxx::builder::document{"_id", id.oid().value()};
            return findOne(queryDoc);
        }

        [[nodiscard]]
        std::optional<Document<Schema>> findById(std::string_view id)
        {
            auto queryDoc = bsoncxx::builder::document{"_id", bsoncxx::oid(id)};
            return findOne(queryDoc);
        }


        /**
         * Finds a list of documents that match the bson query.
         *
         * @param query - bson document that can be built like so:
         *                `{"email", "joe@joe.com"}`
         *
         * @return a list of Document objects if found, or an empty one if
         *         none match the query.
         */
        [[nodiscard]]
        std::vector<Document<Schema>> find(bson query)
        {
            auto docs = m_collection.find(query.view().get_document().value);
            std::vector<Document<Schema>> res;

            for (auto &doc : docs)
                res.emplace_back(Document<Schema>::fromBson(doc));

            return res;
        }


        /**
         * Delete the first document that matches the bson query
         *
         * @param  query - bson document that can be built like so:
         *                `{"email", "joe@joe.com"}`
         *
         * @return       whether a document was deleted
         */
        bool deleteOne(bson query)
        {
            auto result = m_collection.delete_one(query.view().get_document().value);
            return result && result.value().deleted_count() == 1;
        }

        /**
         * Delete all documents that match the bson query
         *
         * @param  query - bson document that can be built like so:
         *                `{"email", "joe@joe.com"}`
         *
         * @return       whether any document was deleted
         */
        bool deleteMany(bson query)
        {
            auto result = m_collection.delete_many(query.view().get_document().value);
            return result && result.value().deleted_count() > 0;
        }

    private:
        mongocxx::collection m_collection;
    };

}
