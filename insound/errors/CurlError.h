#pragma once

#include <stdexcept>

namespace Insound {
    class CurlError : public std::runtime_error
    {
    public:
        explicit CurlError(int curlError);
    };
}
