#include "env.h"

#include <insound/core/definitions.h>
#include <insound/core/errors/MissingEnvError.h>
#include <insound/core/log.h>

#include <cstdlib>
#include <stdexcept>
#include <string>

#include <fstream>

namespace Insound {
    std::string_view getEnv(const std::string_view &varName)
    {
        const auto var = std::getenv(varName.data());
        return var ? var : "";
    }

    std::string_view getEnv(const std::string_view &varName, std::string_view defaultValue)
    {
        const auto var = std::getenv(varName.data());
        return var ? var : defaultValue;
    }

    std::string_view getEnv(std::string_view varName, std::string_view defaultValue)
    {
        const auto var = std::getenv(varName.data());
        return var ? var : defaultValue;
    }

    std::string_view getEnv(std::string_view varName, const char *defaultVal)
    {
        const auto var = std::getenv(varName.data());
        return var ? var : (defaultVal ? defaultVal : "");
    }

    std::string_view requireEnv(std::string_view varName)
    {
        const auto var = std::getenv(varName.data());
        if (!var)
            throw MissingEnvError(varName.data());
        return var;
    }

    /**
     * Set relative paths from project root. Absolute paths are unaffected.
     */
    static std::string makePath(std::string_view path)
    {
        if (path.empty() || path.starts_with('/')) return path.data();
        if (path.starts_with("./")) return f("{}{}", PROJECT_ROOT, path.substr(1));

        return f("{}/{}", PROJECT_ROOT, path);
    }

    void configureEnv(std::string_view path)
    {
        // Open env file
        auto file = std::fstream(makePath(path));
        if (!file.is_open())
        {
            IN_WARN("Attempted to configure env but file could not be opened");
            return;
        }

        // Visit each line of file
        while (true)
        {
            // get current line
            std::string line;
            std::getline(file, line);

            // exit condition
            if (file.eof() || file.bad()) {
                break;
            }

            // skip empty lines
            if (line.empty() || line[0] == '#') continue;

            // read name and value
            auto eqPos = line.find_first_of('=');
            std::string name = line.substr(0, eqPos);
            std::string value = line.substr(eqPos+1);

            // commit to environment
            setenv(name.c_str(), value.c_str(), true);
        }
    }

    void resetEnv(std::string_view path)
    {
        // Open env file
        auto file = std::fstream(makePath(path));
        if (!file.is_open())
        {
            IN_WARN("Attempted to configure env but file could not be opened");
            return;
        }

        // Visit each line of file
        while (true)
        {
            // get current line
            std::string line;
            std::getline(file, line);

            // exit condition
            if (file.eof() || file.bad()) break;

            // skip empty lines
            if (line.empty()) continue;

            // read name & unset
            auto eqpos = line.find_first_of('=');
            std::string name = line.substr(0, eqpos);

            unsetenv(name.c_str());
        }
    }
}
