#pragma once
#include <string>


namespace Insound {

    [[nodiscard]]
    std::string hash(const std::string &text);

    [[nodiscard]]
    bool compare(const std::string &text, const std::string &encrypted);
}
