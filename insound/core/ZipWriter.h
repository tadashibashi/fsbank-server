#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct zip_t;

namespace Insound {

    class ZipBuffer {
    public:
        ZipBuffer() : m_buffer(), m_size() { }
        ~ZipBuffer();

        void *data() { return m_buffer; }

        size_t size() { return m_size; }

        void *begin() { return m_buffer; }
        void *end() { return m_buffer + m_size; }
        const void *begin() const { return m_buffer; }
        const void *end() const { return m_buffer + m_size; }
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
        void addFile(const std::string &key, const std::string &file);

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
