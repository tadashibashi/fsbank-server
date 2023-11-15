/**
 * @file chrono.h
 *
 * Contains literals for converting units of time into microseconds.
 */
#pragma once

namespace Insound::ChronoLiterals {
    /**
     * Get the number of microseconds in the provided milliseconds
     */
    long long operator ""_ms(long double ms);
    long long operator ""_ms(unsigned long long ms);

    /**
     * Get the number of microseconds in the provided seconds
     */
    long long operator ""_s(long double s);
    long long operator ""_s(unsigned long long s);

    /**
     * Get the number of microseconds in the provided minutes
     */
    long long operator ""_min(long double min);
    long long operator ""_min(unsigned long long min);

    /**
     * Get the number of microseconds in the provided hours
     */
    long long operator ""_h(long double h);
    long long operator ""_h(unsigned long long h);

    /**
     * Get the number of microseconds in the provided weeks
     */
    long long operator ""_w(long double w);
    long long operator ""_w(unsigned long long w);

    /**
     * Get the number of microseconds in the provided days
     */
    long long operator ""_d(long double d);
    long long operator ""_d(unsigned long long d);

    /**
     * Get the number of microseconds in the provided years.
     * Avg year is 365.25 days, so this value is used.
     */
    long long operator ""_y(long double y);
    long long operator ""_y(unsigned long long y);
}
