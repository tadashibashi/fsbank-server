#pragma once
#include <stdexcept>
#include <string_view>

namespace Insound {
    class MissingEnvError : public std::runtime_error {
    public:
        MissingEnvError(std::string_view varName) :
            std::runtime_error(
                f("Missing Environment Variable: \"{}\"", varName))
        { }
    };
}
