#include "password.h"
#include <bcrypt.h>

namespace Insound {

    std::string hash(std::string_view text)
    {
        return bcrypt::generateHash(text);
    }

    bool compare(std::string_view text, std::string_view encrypted)
    {
        return bcrypt::validatePassword(text, encrypted);
    }
}
