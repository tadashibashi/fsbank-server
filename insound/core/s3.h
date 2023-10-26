#pragma once
#include <string>
#include <vector>

namespace Insound::S3 {
    /**
     * Initialize and configure S3 API – must be called successfully before any
     * other function in the S3 namespace
     *
     * @return whether init/configuration was successful.
     */
    bool config();

    /**
     * Clean up the S3 API.
     */
    void close();

    /**
     * Get a list of object keys in the store, stemming from `prefix`
     *
     * @param   prefix - the prefix from which to stem the search for objects
     *
     * @returns the list of key names
     */
    std::vector<std::string> listObjects(const std::string_view &prefix = "");

    /**
     * Upload a file to the project's s3 bucket
     *
     * @param    key       the key path to store the object at
     * @param    file      data to store
     *
     * @return   whether upload was successful
     */
    bool uploadFile(const std::string_view &key,
        const std::string &file);
    // std::vector<uint8_t> downloadFile(const std::string_view &key);

}
