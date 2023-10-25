#pragma once
#include <string>
#include <vector>

namespace Insound::S3 {
    bool config();
    void close();

    std::vector<std::string> listObjects();

    // bool uploadFile(const std::string_view &key,
    //     const std::vector<uint8_t> &file);
    // std::vector<uint8_t> downloadFile(const std::string_view &key);

}
