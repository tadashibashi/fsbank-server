#pragma once
#include <stdexcept>
#include <string>

namespace Insound {
    class MissingEnvError : public std::runtime_error {
    public:
        MissingEnvError(const std::string &varName) :
            std::runtime_error(f("Missing Environment Variable: {}", varName))
        { }
    };
}
