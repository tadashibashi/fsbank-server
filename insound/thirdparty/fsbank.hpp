#pragma once
#include <insound/log.h>

#include <fsbank.h>
#include <fsbank_errors.h>

/**
 * Macro that checks an FSBank function. It causes a return on error with a
 * cstring containing the error message. Inspired by blargg gme.
 * @param  result Result of the FSBank function
 */
#define FSB_CHECK(result) do { \
        const FSBANK_RESULT res = (result); \
        const char *message = res ? FSBank_ErrorString(res) : nullptr; \
        if ( (res) != FSBANK_OK ) { \
            IN_ERR("FSBank Error: {}", message); \
            return message; \
        } \
    } while(0)
