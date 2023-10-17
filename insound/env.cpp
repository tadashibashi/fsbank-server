#include "env.h"
#include "definitions.h"
#include "errors/MissingEnvError.h"
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <fstream>
#include <iostream>

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
        auto file = std::fstream(makePath(path));
        if (!file.is_open())
        {
            std::cerr << "Attempted to configure env but file could not be opened\n";
            return;
        }

        // Visit each line of file & set environment variables
        while (true)
        {
            std::string line;
            std::getline(file, line);
            if (file.eof() || file.bad()) {
                break;
            }

            if (line.empty()) continue;

            auto eqPos = line.find_first_of('=');
            std::string name = line.substr(0, eqPos);
            std::string value = line.substr(eqPos+1);

            setenv(name.c_str(), value.c_str(), true);
        }
    }

    void resetEnv(const std::string &path)
    {
        auto file = std::fstream(makePath(path));
        if (!file.is_open())
        {
            std::cerr << "Attempted to configure env but file could not be opened\n";
            return;
        }

        // Visit each line of file & unset environment variables
        while (true)
        {
            std::string line;
            std::getline(file, line);
            if (file.eof() || file.bad()) break;

            if (line.empty()) continue;

            auto eqpos = line.find_first_of('=');
            std::string name = line.substr(0, eqpos);

            unsetenv(name.c_str());
        }
    }
}
