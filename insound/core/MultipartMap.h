/**
 * @file MultipartMap.h
 *
 * Contains MultipartMap class, which is used to store form data from a
 * request.
 */
#pragma once
#include <crow/http_request.h>

#include <map>
#include <string>

namespace Insound {

    /**
     * Data structure for files parsed in multipart/form-data requests.
     */
    struct FileData {
        /**
         * The filename specified by the user. It should be cleaned.
         */
        std::string filename;

        /**
         * File data stored in a string container. It is not necessarily a
         * c-string and may contain binary data. (Its length is still correct)
         */
        std::string data;
    };

    /**
     * Container class that stores a map of parsed posted form data.
     */
    class MultipartMap {
    public:
        MultipartMap() = default;

        /**
         * Create a multipart map from a request with one of the following
         * content types:
         * - multipart/form-data
         * - application/x-www-form-urlencoded
         * - application/json
         *
         * Throws a runtime_error if the request contains none of the listed
         * supported content types, or if a content-type header was not found.
         *
         * Throws an invalid_argument exception if the body has a problem with
         * it.
         */
        static MultipartMap from(const crow::request &req);

        /**
         * Get the byte size of all files
         */
        [[nodiscard]]
        size_t fileSize() const;

        /**
         * Shorthand for string map
         */
        using Map = std::map<std::string, std::string>;

        /**
         * Contains parsed form text fields.
         * The key is the fieldname, and the value is the value.
         */
        std::map<std::string, std::string> fields;

        /**
         * Contains parsed form files.
         * The key is the fieldname, and the value is FileData type, which
         * contains the filename and data buffer.
         */
        std::map<std::string, FileData> files;
    };
}
