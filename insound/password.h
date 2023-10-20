#pragma once
#include <string>


namespace Insound {

    /**
     * Encrypt a text string
     * @param text - the string to encrypt
     */
    [[nodiscard]]
    std::string hash(const std::string &text);


    /**
     * Compare text to a string that was encrypted via Insound::hash
     * @param   text      - the text to compare
     * @param   encrypted - the encrypted string
     * @returns true when text matches encrypted string, and false otherwise.
     */
    [[nodiscard]]
    bool compare(const std::string &text, const std::string &encrypted);
}
