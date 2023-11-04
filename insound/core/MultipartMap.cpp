#include "MultipartMap.h"
#include "crow/json.h"
#include <crow/multipart.h>

#include <optional>
#include <string_view>
#include <utility>

namespace Insound {

    /**
     * Helper to decode a URL-encoded string. Specifically, it parses all `%HH`
     * to its char value, and converts '+' to ' '.
     * @param  in - input url-encoded string
     * @return      parsed string, e.g. "bob%40mail.com" -> "bob@mail.com"
     */
    static inline std::string decodeUrl(const std::string_view &in)
    {
        std::string out;
        auto length = in.length();
        for (size_t i = 0; i < length;)
        {
            switch(in[i])
            {
                case '%':
                {
                    if (i >= length - 2)
                        throw std::invalid_argument(f("Malformed URL-encoded "
                            "string: escaped char at position {} is missing "
                            "hex character(s)", i));
                    auto end = &in[i+2];
                    out += (unsigned char)std::strtoul(&in[i+1], (char **)&end, 16);
                    i += 3;
                    break;
                }
                case '+':
                    out += ' ';
                    ++i;
                    break;

                default:
                    out += in[i];
                    ++i;
                    break;
            }
        }

        return out;
    }

    /**
     * Handles a JSON response, creating a MultipartMap of first-level
     * key-value strings. (Nested JSON values are not supported)
     */
    static inline MultipartMap handleJSON(const crow::request &req)
    {
        try {
            MultipartMap map;
            auto json = crow::json::load(req.body);
            for (auto &value : json)
            {
                map.fields[value.key()] = value.s();
            }

            return map;
        }
        catch(const std::runtime_error &e)
        {
            // Unify error type, using invalid_argument for invalid body.
            // runtime_error is thrown by crow::json when encountering
            // parsing errors.
            throw std::invalid_argument(e.what());
        }
    }

    /**
     * Handles a multipart form data response, converting it to a MultipartMap.
     * (uses crow::multipart::message under the hood.)
     */
    static inline MultipartMap handleMultipart(const crow::request &req)
    {
        std::optional<crow::multipart::message> message{};

        try {
            message = crow::multipart::message(req);

            // Exception here is most-likely due to invalid request.
            // We'll make that assumption until a bug is proven to be found in
            // the crow codebase.
        }
        catch(const std::exception &e)
        {
            throw std::invalid_argument(f("Error occurred while parsing "
                "multipart message: {}", e.what()));
        }
        catch(...)
        {
            throw std::invalid_argument("Unknown error occurred while parsing "
                "multipart message.");
        }

        MultipartMap map;

        // visit each part
        for (const auto &[name, value] : message.value().part_map)
        {
            auto contentdisp_it = value.headers.find("Content-Disposition");
            if (contentdisp_it == value.headers.end())
            {
                throw std::invalid_argument(f("Multipart map part \"{}\" is "
                    "missing a Content-Disposition header.", name));
            }

            // check for filename header
            auto filename_it = contentdisp_it->second.params.find("filename");
            if (filename_it == contentdisp_it->second.params.end())
            {
                // no filename in header -> store the text field
                map.fields[name] = std::move(value.body);
            }
            else
            {
                // filename in header found -> store the file
                FileData fileData;
                fileData.filename = std::move(filename_it->second);
                fileData.data = std::move(value.body);

                map.files[name] = std::move(fileData);
            }
        }

        return map;
    }

    /**
     * Parses urlencoded form data from a request into a MultipartMap.
     */
    static inline MultipartMap handleFormUrlEncoded(const crow::request &req)
    {
        // Decode the body
        auto body = decodeUrl(req.body);
        MultipartMap map;

        // Scan body for key-value pairs
        for (size_t i = 0, size = body.size(); i < size;)
        {
            // Find the current '='
            auto eq = body.find_first_of('=', i);
            if (eq == std::string::npos) // weird case: key missing value
                throw std::invalid_argument("Url-encoded form error: key "
                    "is missing a value.");

            // Find the end of this value
            auto end = body.find_first_of('&', i);

            // Get and set kvp
            auto key = body.substr(i, eq-i);
            auto value = body.substr(eq+1, end-1-eq);

            map.fields[key] = value;

            // Move to next kvp, or quit if end-of-body
            if (end == std::string::npos)
                break;
            i = end + 1; // move i one past the '&'
        }

        return map;
    }

    MultipartMap MultipartMap::from(const crow::request &req)
    {
        // Find content-type
        auto contenttype_it = req.headers.find("Content-Type");
        if (contenttype_it == req.headers.end())
            throw std::invalid_argument("MultipartMap::from: no content-type "
                "in crow::request headers");

        // Handle request body based on content-type
        if (contenttype_it->second.starts_with("application/json"))
        {
            return handleJSON(req);
        }
        else if (contenttype_it->second.starts_with("multipart/form-data"))
        {
            return handleMultipart(req);
        }
        else if (contenttype_it->second.starts_with(
            "application/x-www-form-urlencoded"))
        {
            return handleFormUrlEncoded(req);
        }

        // Not a supported content-type
        throw std::runtime_error("MultipartMap::from: content-type not "
            "recognized: " + contenttype_it->second);
    }
}


