#pragma once
#include <crow/http_request.h>

#include <map>
#include <string>

namespace Insound {
    struct FileData {
        std::string filename;
        std::string data;
    };

    /**
     * Container class that stores a map of parsed multi-part form data from
     * a request. Supports application/json but not urlencoded form format.
     */
    class MultipartMap {
    public:
        MultipartMap();

        static MultipartMap from(const crow::request &req);

        using Map = std::map<std::string, std::string>;

        std::map<std::string, std::string> fields;
        std::map<std::string, FileData> files;
    };
}
