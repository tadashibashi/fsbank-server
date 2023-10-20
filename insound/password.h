#pragma once
#include <string>


namespace Insound {

    /**
     * Encrypt a text string
     */
    [[nodiscard]]
    std::string hash(const std::string &text);


    /**
     * Compare text to a string that was encrypted via Insound::hash
     * @returns true when text matches encrypted string, and false otherwise.
     */
    [[nodiscard]]
    bool compare(const std::string &text, const std::string &encrypted);
}
