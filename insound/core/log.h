/**
 * @file log.h
 *
 * Contains definitions for debug logging.
 */
#pragma once

#define INSOUND_ALLOW_LOGGING
#if defined(_DEBUG) || defined(INSOUND_ALLOW_LOGGING)

#include <insound/core/thirdparty/spdlog.hpp>
namespace Insound::Log {
    std::shared_ptr<spdlog::logger> appLogger();
}

#define IN_LOG(...)  Insound::Log::appLogger()->info(__VA_ARGS__)
#define IN_WARN(...) Insound::Log::appLogger()->warn(__VA_ARGS__)
#define IN_ERR(...)  Insound::Log::appLogger()->error(__VA_ARGS__)

#else

// No debug logs in release mode

#define IN_LOG(...)  (void)0
#define IN_WARN(...) (void)0
#define IN_ERR(...)  (void)0

#endif
