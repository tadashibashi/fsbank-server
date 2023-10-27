#pragma once
#include <crow/http_request.h>

#include <map>
#include <string>

namespace Insound {
    /**
     * Class that stores multipart form data from a request via
     * crow::multipart::message, for quick access in a route.
     */
    class MultipartMap {
    public:
        explicit MultipartMap(const crow::request &req);
        using Map = std::map<std::string, std::string>;

        Map fields;
        Map files;
    };
}
