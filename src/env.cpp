#include "env.h"
#include "errors/MissingEnvError.h"
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace Insound {

    const char *getEnv(const char *varName, const char *defaultValue)
    {
        const auto var = std::getenv(varName);
        return var ? var : defaultValue;
    }

    const char *requireEnv(const char *varName)
    {
        const auto var = std::getenv(varName);
        if (!var)
            throw MissingEnvError(varName);
        return var;
    }
}
