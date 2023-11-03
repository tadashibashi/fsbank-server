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

    std::vector<uint8_t> openFile(std::string_view path)
    {
        // open file
        std::ifstream file(path.data(), std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            throw std::runtime_error(f("Failed to open file at path {}", path));
        }

        // find file size
        file.seekg(0, std::ios::end);
        auto filesize = file.tellg();
        file.seekg(0);

        // read file into buffer
        std::vector<uint8_t> buffer(filesize, 0);
        file.read((char *)buffer.data(), filesize);

        return buffer;
    }

    std::string toUpper(std::string_view str)
    {
        std::string res;
        res.reserve(str.length());

        for (char c : str)
            res += std::toupper(c);
        return res;
    }

} // namespace Insound
