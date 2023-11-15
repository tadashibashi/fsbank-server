/**
 * @file mongo.h
 *
 * Contains global functions for dealing with MongoDB. Use classes in mongo/
 * directory for abstracted usage.
 */
#pragma once
#include <mongocxx/database.hpp>

namespace Insound::Mongo
{
    /**
     * Get Mongo database object. Thread-safe.
     */
    mongocxx::database db();

    /**
     * Connect to MongoDB. Must be called successfully before any Client is
     * created. Safe to call if already connected, will simply return true.
     *
     * @returns whether call was successful.
     */
    bool connect();
}
