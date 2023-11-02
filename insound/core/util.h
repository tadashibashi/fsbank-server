#pragma once
#include <string>
#include <vector>

namespace Insound {

    /**
     * Generate a random hex string, useful for id
     *
     * @param  length - the number of chars to generate, default=16
     */
    std::string genHexString(unsigned int length=16);

    /**
     * Generate a list of random bytes
     *
     * @param  length - the number of bytes to generate, default=16
     */
    std::vector<unsigned char> genBytes(unsigned int length=16);

    /**
     * Open a file and retrieve its contents as a vector of bytes
     */
    std::vector<uint8_t> openFile(const std::string &path);

    /**
     * Create an uppercase version of a string.
     * @param  str - string view to make uppercase
     * @return uppercase string.
     */
    std::string toUpper(const std::string_view &str);
}
