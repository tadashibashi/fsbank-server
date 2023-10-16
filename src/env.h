#pragma once

namespace Insound {
    /**
     * Get a variable value from the environment, or a default value if it
     * doesn't exist. This value is set to nullptr by default.
     *
     * @param  varName - name of the environment variable
     */
    const char *getEnv(const char *varName, const char *defaultVal = nullptr);

    /**
     * Get an environment variable, or throw an error if it doesn't exist.
     *
     * @param  varName - name of the environment variable
     */
    const char *requireEnv(const char *varName);
}
