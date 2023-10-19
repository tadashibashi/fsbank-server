#include "env.h"

#include <insound/definitions.h>
#include <insound/errors/MissingEnvError.h>
#include <insound/log.h>

#include <cstdlib>
#include <stdexcept>
#include <string>

#include <fstream>

namespace Insound {

    std::string getEnv(const std::string &varName, const std::string &defaultValue)
    {
        const auto var = std::getenv(varName.c_str());
        return var ? var : defaultValue;
    }

    std::string requireEnv(const std::string &varName)
    {
        const auto var = std::getenv(varName.c_str());
        if (!var)
            throw MissingEnvError(varName);
        return var;
    }

    /**
     * Set relative paths from project root. Absolute paths are unaffected.
     */
    static std::string makePath(const std::string &path)
    {
        if (path.empty() || path.starts_with('/')) return path;
        if (path.starts_with("./")) return PROJECT_ROOT + path.substr(1);

        return PROJECT_ROOT "/" + path;
    }

    void configureEnv(const std::string &path)
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
            if (line.empty()) continue;

            // read name and value
            auto eqPos = line.find_first_of('=');
            std::string name = line.substr(0, eqPos);
            std::string value = line.substr(eqPos+1);

            // commit to environment
            setenv(name.c_str(), value.c_str(), true);
        }
    }

    void resetEnv(const std::string &path)
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
