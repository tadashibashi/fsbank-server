#include "MultipartMap.h"
#include <crow/multipart.h>
#include <utility>

namespace Insound {

    MultipartMap::MultipartMap() :
        fields(), files()
    {
    }

    MultipartMap MultipartMap::from(const crow::request &req)
    {
        // Check that this is a multipart map request
        {
            auto contenttype_it = req.headers.find("Content-Type");
            if (contenttype_it == req.headers.end())
                throw std::runtime_error("MultipartMap::from: no content-type "
                    "in crow::request headers");
            if (!contenttype_it->second.starts_with("multipart/form-data"))
                throw std::runtime_error("MultipartMap::from: content-type is "
                    "not \"multipart/form-data\"");
        }

        auto message = crow::multipart::message(req);
        MultipartMap map;

        // visit each part
        for (const auto &[name, value] : message.part_map)
        {
            auto contentdisp_it = value.headers.find("Content-Disposition");
            if (contentdisp_it == value.headers.end())
            {
                IN_WARN("Multipart header contains part \"{}\" with no "
                    "Content-Disposition header.", name);
                continue;
            }

            // check for filename header
            auto filename_it = contentdisp_it->second.params.find("filename");
            if (filename_it == contentdisp_it->second.params.end())
            {
                // no filename header: it's a text field
                map.fields[name] = std::move(value.body);
            }
            else
            {
                // filename header found: it's a file
                FileData fileData;
                fileData.filename = std::move(filename_it->second);
                fileData.data = std::move(value.body);

                map.files[name] = std::move(fileData);
            }
        }

        return map;
    }
}


