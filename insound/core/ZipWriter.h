/**
 * @file ZipWriter.h
 *
 * Contains class `ZipWriter` for creating zip files in-memory, that can then
 * be uploaded to S3 or written elsewhere, etc.
 */
#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

// Forward declaration
struct zip_t;

namespace Insound {

    /**
     * Exception-safe container for a zip file buffer
     */
    class ZipBuffer
    {
    public:
        ZipBuffer() : m_buffer(), m_size() { }
        ZipBuffer(const ZipBuffer &buf);
        ZipBuffer(ZipBuffer &&buf);
        ZipBuffer &operator=(const ZipBuffer &buf);
        ZipBuffer &operator=(ZipBuffer &&buf);

        ~ZipBuffer();

        void *data() { return m_buffer; }

        size_t size() { return m_size; }

        char *begin() { return m_buffer; }
        char *end() { return m_buffer + m_size; }
        const void *begin() const { return m_buffer; }
        const void *end() const { return m_buffer + m_size; }

        /**
         * Free the buffer in advance before object destructor is called.
         */
        void close();
    private:
        friend class ZipWriter;
        char *m_buffer;
        size_t m_size;
    };

    class ZipWriter
    {
    public:
        ZipWriter();
        ~ZipWriter();


        /**
         * Add a file to the zip archive
         * @param key  - pathname of file in the zip archive
         * @param file - data of file as a string, it may contain binary data
         */
        void addFile(std::string_view key, const std::string &file);

        // /**
        //  * Delete a file in the zip archive
        //  * @param  key - pathname of file in the zip archive
        //  * @return whether deletion was successful. Logs warning if file
        //  * does not exist, and error if a libzip error occurred.
        //  */
        // bool deleteFiles(const std::vector<std::string> &keys);


        /**
         * Get number of file entries in zip file.
         */
        [[nodiscard]]
        ssize_t numEntries() const;


        /**
         * Return the zip file as a binary string
         */
        [[nodiscard]]
        ZipBuffer copy() const;

        /**
         * Close file ahead of time. Do not call any other ZipFile member
         * functions after a call to this function.
         */
        void close();

    private:
        zip_t *zip;
    };



}
