#pragma once
#include <string>
#include <string_view>

namespace Insound {
    /**
     * Get a variable value from the environment, or a default value if it
     * doesn't exist. This value is set to a blank string by default.
     *
     * @param  varName - name of the environment variable
     */
    std::string_view getEnv(std::string_view varName);
    std::string_view getEnv(std::string_view varName, std::string_view defaultVal);
    std::string_view getEnv(std::string_view varName, const char *defaultVal);

    /**
     * Get a variable value from the environment, or the default value if it
     * does not exist. This function must be specialized with type `T` in order
     * to compile.
     *
     * @tparam `T` - the type to cast to
     * @param  varName - the environment variable name
     * @param  defaultValue - the default value to return should the variable
     *                         exist in the environment.
     * @example
     * // Automatically infer the type
     * `auto PORT = getEnv("MY_PORT_VAR", 1234);`
     *
     * // Explicit set: (96 is casted to float)
     * `auto TEMPERATURE = getEnv<float>("TEMPERATURE", 96);`
     */
    template <typename T>
    T getEnv(std::string_view varName, T defaultVal);


    /**
     * Get an environment variable, or throw an error if it doesn't exist.
     *
     * @param  varName - name of the environment variable
     */
    std::string_view requireEnv(std::string_view varName);


    /**
     * Configure environment from local file, in the format of a typical .env
     * @param path - path to file - relative path stems from project root -
     *               absolute paths work as well.
     */
    void configureEnv(std::string_view path = ".env");


    /**
     * Unsets all variables otherwise set from a config file.
     * @param path - path to file - relative paths stem from project root -
     *               absolute paths work as well.
     */
    void resetEnv(std::string_view path = ".env");
}

template<>
inline int Insound::getEnv<int>(std::string_view varName, int defaultVal)
{
    auto var = std::getenv(varName.data());
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
inline float Insound::getEnv<float>(std::string_view varName, float defaultVal)
{
    auto var = std::getenv(varName.data());
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
inline double Insound::getEnv<double>(std::string_view varName, double defaultVal)
{
    auto var = std::getenv(varName.data());
    if (!var) return defaultVal;

    try {
        return atof(var);
    }
    catch (...)
    {
        return defaultVal;
    }
}
