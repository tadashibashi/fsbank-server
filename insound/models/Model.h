#pragma once

#include <insound/models/Document.h>
#include <bsoncxx/document/view_or_value.hpp>
#include <mongocxx/collection.hpp>

#include <optional>
#include <string>
#include <vector>

namespace Insound::Mongo {

    template <typename Schema>
    class Model {
    public:
        using bson = bsoncxx::document::view_or_value;

        explicit Model(const std::string &collectionName) :
            m_collection(Mongo::db().collection(collectionName))
        { }

        [[nodiscard]]
        std::optional<Document<Schema>> findOne(const bson &query)
        {
            auto doc = m_collection.find_one(query);
            if (!doc) return;

            return Document<Schema>::fromBson(m_collection.name(), doc);
        }

        [[nodiscard]]
        std::vector<Document<Schema>> find(const bson &query)
        {
            auto docs = m_collection.find(query);
            std::vector<Document<Schema>> res;

            for (auto &doc : docs)
                res.emplace_back(Document<Schema>::fromBson(doc));

            return res;
        }

        bool deleteOne(const bson &query)
        {
            auto result = m_collection.delete_one(query);
            return result && result.value().deleted_count() == 1;
        }

        bool deleteMany(const bson &query)
        {
            auto result = m_collection.delete_many(query);
            return result && result.value().deleted_count() > 0;
        }

    private:
        mongocxx::collection m_collection;
    };

}
