#pragma once

// macOS checks
#if defined(__APPLE__)
    #include <TargetConditionals.h>

    #if TARGET_OS_MAC
        #define INSOUND_PLATFORM_MAC

        // Check CPU type
        #if TARGET_CPU_ARM64
            // M1 & M2 Macs
            #define INSOUND_CPU_ARM64
        #else
            // Intel Macs
            #define INSOUND_CPU_X86_64
        #endif
    #else
        #error "This platform is not supported"
    #endif

// Linux checks
#elif defined (__linux__)
    #define INSOUND_PLATFORM_LINUX

    // Check CPU type
    #if defined(__x86_64__) || defined(_M_X64)
        #define INSOUND_CPU_X86_64
    #else
        // ARM64 not supported on Linux
        #error "This platform's cpu is not supported"
    #endif
#else
    #error "This platform is not supported"
#endif
