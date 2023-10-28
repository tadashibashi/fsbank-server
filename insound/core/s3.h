#pragma once
#include <optional>
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
     * Warning: Permanently erases entire S3 bucket, so that it no longer
     * exists. Used for testing and not advisable during production.
     *
     * @return    whether bucket erasure was successful.
     */
    bool dropBucket__permanent__(const std::string_view &bucket);


    /**
     * Create an S3 bucket.
     *
     * @param  bucket - name of the bucket
     *
     * @return          whether request was successful
     */
    bool createBucket(const std::string_view &bucket);


    /**
     * Get a list of object keys in the store, stemming from `prefix`
     *
     * @param     prefix   - the prefix from which to stem the search for objects
     *
     * @returns              the list of key names
     */
    std::vector<std::string> listObjects(const std::string_view &prefix = "");


    /**
     * Upload a file to the project's s3 bucket
     *
     * @param    key       - the key path to store the object at
     * @param    file      - data to store
     *
     * @return             whether upload was successful
     */
    bool uploadFile(const std::string_view &key,
        const std::string &file);


    /**
     * Download a file and get its data encapsulated in a string.
     * Binary files are permitted, but string functionality will be limited.
     *
     * @param   key     - the key of the file to download
     *
     * @return            the file as a string (easy to return via crow).
     */
    std::optional<std::string> downloadFile(const std::string_view &key);


    /**
     * Delete a file in the project's S3 bucket
     *
     * @param   key     - key of the file to delete
     *
     * @return            whether deletion was a success.
     */
    bool deleteFile(const std::string_view &key);


    /**
     * Delete a list of files n the project's S3 bucket
     *
     * @param   keys     - list of keys to delete
     *
     * @return             whether the deletion was a success.
     */
    bool deleteFiles(const std::vector<std::string> &keys);


    /**
     * Delete S3 objects inside of a folder
     *
     * @param   folderKey     - key of the folder to delete
     * 
     * @return                  whether the deletion was a success.
     */
    bool deleteFolder(const std::string_view &folderKey);


    /**
     * Compress files in an S3 folder for download
     *
     * @param   folderKey     - key of the folder to zip
     *
     * @return                  buffer string containing the binary zip file.
     */
    std::optional<std::string> zipFolder(const std::string_view &folderKey);
}
