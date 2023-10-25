#pragma once
#include <string>
#include <vector>

namespace Insound::S3 {
    bool config();
    void close();

    /**
     * Get a list of object keys in the store, stemming from `prefix`
     * @param   prefix - the prefix from which to stem the search for objects
     *
     * @returns the list of key names
     */
    std::vector<std::string> listObjects(const std::string_view &prefix = "");

    /**
     * Upload a file to the project's s3 bucket
     * @param  key  [description]
     * @param  file [description]
     * @return      [description]
     */
    bool uploadFile(const std::string_view &key,
        const std::vector<uint8_t> &file);
    // std::vector<uint8_t> downloadFile(const std::string_view &key);

}
