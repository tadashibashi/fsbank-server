#include "util.h"

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
}
