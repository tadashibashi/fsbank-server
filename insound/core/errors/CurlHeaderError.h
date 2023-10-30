#pragma once
#include <stdexcept>

namespace Insound {
    class CurlHeaderError : public std::runtime_error
    {
    public:
        explicit CurlHeaderError(int curlError);
    };
}
