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
     * Class that stores multipart form data from a request via
     * crow::multipart::message, for quick access in a route.
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
