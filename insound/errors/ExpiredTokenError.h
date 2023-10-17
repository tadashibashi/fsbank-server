#pragma once
#include <stdexcept>

// Forward declaration
namespace Poco::JWT {
    class Token;
}

namespace Insound {
    class ExpiredTokenError : public std::runtime_error {
    public:
        ExpiredTokenError(const Poco::JWT::Token &token);
    private:
        const Poco::JWT::Token &token;
    };
}
