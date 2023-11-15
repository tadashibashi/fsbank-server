/**
 * @file regex.h
 *
 * Contains regex presets for validation checks.
 */
#pragma once
#include <regex>

namespace Insound::Regex {
    /**
     * Check for a valid email address.
     */
    extern std::regex email;

}
