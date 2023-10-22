#pragma once

#include <insound/models/Document.h>
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
         * @param  collectionName - case-insensitive (made lowercase)
         */
        explicit Model() :
            m_collection()
        {
            auto name = glz::meta<Schema>::name;
            std::string lowercased;
            for (auto c : name)
                lowercased += std::tolower(c);
            if (!Mongo::db().has_collection(lowercased))
                m_collection = Mongo::db().create_collection(lowercased);
            else
                m_collection = Mongo::db().collection(lowercased);


        }

        std::optional<Document<Schema>> insertOne(const Schema &obj)
        {
            auto doc = Document<Schema>(m_collection.name(), obj);
            if (doc.save())
                return doc;
            return {};
        }

        [[nodiscard]]
        std::optional<Document<Schema>> findOne(bson query)
        {
            auto queryDoc = query.view().get_document().value;
            auto doc = m_collection.find_one(query.view().get_document().value);
            if (!doc) return {};

            return Document<Schema>::fromBson(m_collection.name(), doc.value().view());
        }

        [[nodiscard]]
        std::vector<Document<Schema>> find(bson query)
        {
            auto docs = m_collection.find(query.view().get_document().value);
            std::vector<Document<Schema>> res;

            for (auto &doc : docs)
                res.emplace_back(Document<Schema>::fromBson(doc));

            return res;
        }

        bool deleteOne(bson query)
        {
            auto result = m_collection.delete_one(query.view().get_document().value);
            return result && result.value().deleted_count() == 1;
        }

        bool deleteMany(bson query)
        {
            auto result = m_collection.delete_many(query.view().get_document().value);
            return result && result.value().deleted_count() > 0;
        }

    private:
        mongocxx::collection m_collection;
    };

}
