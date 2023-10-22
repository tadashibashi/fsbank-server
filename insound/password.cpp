#include "password.h"
#include <insound/env.h>
#include <insound/log.h>
#include <bcrypt.h>
#include <memory>

namespace Insound {

    std::string hash(const std::string &text)
    {
        return bcrypt::generateHash(text);
    }

    bool compare(const std::string &text, const std::string &encrypted)
    {
        return bcrypt::validatePassword(text, encrypted);
    }
}
