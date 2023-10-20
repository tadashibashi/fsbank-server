#include "log.h"

#if defined(_DEBUG) || defined(INSOUND_ALLOW_LOGGING)
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Insound::Log {
    static std::shared_ptr<spdlog::logger> sAppLogger;

    std::shared_ptr<spdlog::logger> appLogger()
    {
        if (!sAppLogger)
            sAppLogger = spdlog::stdout_color_mt("Insound");

        return sAppLogger;
    }
}

#endif
