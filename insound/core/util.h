#include <string>

namespace Insound {

    /**
     * Generate a random hex string, useful for id
     * @param  length - the number of chars
     */
    std::string genHexString(unsigned int length=16);

    std::vector<unsigned char> genBytes(unsigned int length=16);
}
