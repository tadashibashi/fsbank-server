#pragma once
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/types.hpp>

#include <optional>
#include <string>

namespace Insound::Mongo {
    /**
     * 12-byte MongoDB object id.
     * Consists of
     * - a 4-byte timestamp, representing the id's creation, measured in
     *   seconds since the unix epoch.
     * - a 5-byte random value generated once per process. Unique to the
     *   machine and the process.
     * - a 3-byte incrementing counter, initialized to a random value
     */
    class Id {
    public:
        Id() : m_id() { }

        explicit Id(const std::string &idString) :
            m_id(idString) { }
        explicit Id(const bsoncxx::oid &oid) :
            m_id(oid) { }

        // `doc` must be a value retrieved via mongocxx find_one, find, etc.
        // which contains an _id field
        Id static fromBsonDocument(const bsoncxx::document::view_or_value &doc)
        {
            return Id{doc.view()["_id"].get_oid().value};
        }

        /**
         * Get the number of seconds since the unix epoch: January 1, 1970
         */
        [[nodiscard]]
        time_t createdAt() const
        {
            return m_id ? m_id.value().get_time_t() : -1L;
        }

        /**
         * Get the inner id as a hex string.
         */
        [[nodiscard]]
        std::string str() const { return m_id ? m_id.value().to_string() : ""; }

        /**
         * Get the raw bson oid object.
         */
        [[nodiscard]]
        const std::optional<bsoncxx::oid> &oid() const { return m_id; }

        [[nodiscard]]
        bool operator==(const Id &other) const { return m_id == other.m_id; }

        [[nodiscard]]
        bool operator==(const std::string &other) const
        {
            return m_id.has_value() &&
                std::strcmp(m_id.value().bytes(), other.c_str()) == 0;
        }

        [[nodiscard]]
        bool operator==(const char *other) const
        {
            return m_id.has_value() &&
                std::strcmp(m_id.value().bytes(), other) == 0;
        }

        [[nodiscard]]
        bool operator==(const bsoncxx::oid &oid) const
        {
            return m_id.has_value() && oid == m_id;
        }

        [[nodiscard]]
        operator bool() const { return m_id.has_value(); }

    private:
        std::optional<bsoncxx::oid> m_id;
    };

    [[nodiscard]]
    inline bool operator==(const std::string &a, const Id &b)
    {
        return b.operator==(a);
    }

    [[nodiscard]]
    inline bool operator==(const char *a, const Id &b)
    {
        return b.operator==(a);
    }

    [[nodiscard]]
    inline bool operator==(const bsoncxx::oid &a, const Id &b)
    {
        return b.operator==(a);
    }
}
