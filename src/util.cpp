#include "util.h"

namespace Insound {
    static std::string HexChars = "0123456789abcdef";
    static const int HexLength = HexChars.size();

    std::string gen_hex_string(unsigned int length)
    {
        auto str = std::string();
        str.reserve(length);

        for (unsigned i = 0; i < length; ++i)
            str += HexChars[rand() % HexLength];

        return str;
    }
}
