/**
 * @file password.h
 *
 * Contains functions for hashing passwords and comparing hashed values.
 */
#pragma once
#include <string>
#include <string_view>

namespace Insound {

    /**
     * Encrypt a text string
     * @param text - the string to encrypt
     */
    [[nodiscard]]
    std::string hash(std::string_view text);


    /**
     * Compare text to a string that was encrypted via Insound::hash
     * @param   text      - the text to compare
     * @param   encrypted - the encrypted string
     * @returns true when text matches encrypted string, and false otherwise.
     */
    [[nodiscard]]
    bool compare(std::string_view text, std::string_view encrypted);
}
