#pragma once
#include <string>

namespace Insound {
    /**
     * Get a variable value from the environment, or a default value if it
     * doesn't exist. This value is set to a blank string by default.
     *
     * @param  varName - name of the environment variable
     */
    std::string getEnv(const std::string &varName, const std::string &defaultVal = std::string());

    template <typename T>
    T getEnvType(const std::string &varName, T defaultVal);

    /**
     * Get an environment variable, or throw an error if it doesn't exist.
     *
     * @param  varName - name of the environment variable
     */
    std::string requireEnv(const std::string &varName);

    /**
     * Configure environment from local file, in the format of a typical .env
     * @param path - path to file - relative path stems from project root -
     *               absolute paths work as well.
     */
    void configureEnv(const std::string &path = ".env");

    /**
     * Unsets all variables otherwise set from a config file.
     * @param path - path to file - relative paths stem from project root -
     *               absolute paths work as well.
     */
    void resetEnv(const std::string &path = ".env");
}

template<>
inline int Insound::getEnvType<int>(const std::string &varName, int defaultVal)
{
    auto var = std::getenv(varName.c_str());
    if (!var) return defaultVal;

    try {
        return atoi(var);
    }
    catch (...)
    {
        return defaultVal;
    }
}

template<>
inline float Insound::getEnvType<float>(const std::string &varName, float defaultVal)
{
    auto var = std::getenv(varName.c_str());
    if (!var) return defaultVal;

    try {
        return atof(var);
    }
    catch (...)
    {
        return defaultVal;
    }
}

template<>
inline double Insound::getEnvType<double>(const std::string &varName, double defaultVal)
{
    auto var = std::getenv(varName.c_str());
    if (!var) return defaultVal;

    try {
        return atof(var);
    }
    catch (...)
    {
        return defaultVal;
    }
}
