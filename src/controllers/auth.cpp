#include "../server.h"
#include <crow.h>

namespace Insound::Auth {
    crow::response post_login(const crow::request &req)
    {
        //std::cout << req.headers.find("content-type")->second << '\n';
        auto msg = crow::multipart::message(req);
        auto password = msg.get_part_by_name("password").body;
        auto email = msg.get_part_by_name("email").body;

        // it looks like you can't stack files, so they each need a unique name
        auto file = msg.get_part_by_name("file0").body;
        auto file2 = msg.get_part_by_name("file1").body;

        for (const auto &part : msg.part_map) {
            const auto &part_name = part.first;
            const auto &part_value = part.second;

            // get filename
            auto headers_it = part_value.headers.find("Content-Disposition");
            if (headers_it == part_value.headers.end())
            {
                CROW_LOG_ERROR << "No Content-Disposition found";
                return crow::response(400);
            }

            auto params_it = headers_it->second.params.find("filename");
            if (params_it == headers_it->second.params.end())
            {
                // It's a text part
                std::cout << "part: " << part_name <<  ", value: " << part_value.body << '\n';
            }
            else
            {
                // It's a file
                std::cout << "part: " << part_name << ", filename: " << params_it->second << '\n';
            }
        }

        return crow::response("Password: " + password + ", Email: " + email + ", File: " + file2);
    }
}
