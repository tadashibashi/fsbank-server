#include "util.h"
#include <fstream>

namespace Insound {
    /**
     * Map of hex chars to string indices
     */
    static const char *HexChars = "0123456789abcdef";
    static constexpr int HexLength = 16;

    std::string genHexString(unsigned int length)
    {
        auto str = std::string();
        str.reserve(length);

        for (unsigned i = 0; i < length; ++i)
            str += HexChars[rand() % HexLength];

        return str;
    }

    std::vector<unsigned char> genBytes(unsigned int length)
    {
        std::vector<unsigned char> res;
        res.reserve(length);
        for (unsigned i = 0; i < length; ++i)
            res.emplace_back(rand() % UCHAR_MAX);
        return res;
    }

    std::vector<uint8_t> openFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file at path " + path);
        }

        file.seekg(0, std::ios::end);
        auto filesize = file.tellg();
        std::vector<uint8_t> buffer(filesize, 0);
        file.seekg(0);
        file.read((char *)buffer.data(), filesize);

        file.close();
        return buffer;
    }

} // namespace Insound
