#include "chrono.h"

namespace Insound::ChronoLiterals {

    long long operator ""_ms(long double ms)
    {
        return static_cast<long long>(ms * 1000LL);
    }
    long long operator ""_ms(unsigned long long ms)
    {
        return static_cast<long long>(ms * 1000LL);
    }

    long long operator ""_s(long double seconds)
    {
        return static_cast<long long>(seconds * 1000000LL);
    }
    long long operator ""_s(unsigned long long seconds)
    {
        return static_cast<long long>(seconds * 1000000LL);
    }

    long long operator ""_min(long double min)
    {
        return static_cast<long long>(min * 60000000LL);
    }
    long long operator ""_min(unsigned long long min)
    {
        return static_cast<long long>(min * 60000000LL);
    }

    long long operator ""_h(long double h)
    {
        return static_cast<long long>(h * 3600000000LL);
    }
    long long operator ""_h(unsigned long long h)
    {
        return static_cast<long long>(h * 3600000000LL);
    }

    long long operator ""_d(long double d)
    {
        return static_cast<long long>(d * 86400000000LL);
    }
    long long operator ""_d(unsigned long long d)
    {
        return static_cast<long long>(d * 86400000000LL);
    }

    long long operator ""_w(long double w)
    {
        return static_cast<long long>(w * 604800000000LL);
    }
    long long operator ""_w(unsigned long long w)
    {
        return static_cast<long long>(w * 604800000000LL);
    }

    long long operator ""_y(long double y)
    {
        return static_cast<long long>(y * 31557600000000LL);
    }
    long long operator ""_y(unsigned long long y)
    {
        return static_cast<long long>(y * 31557600000000LL);
    }
}
