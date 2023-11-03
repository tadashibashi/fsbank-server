#pragma once
#include <stdexcept>
#include <glaze/core/common.hpp>

namespace Insound {

    class GlazeError : public std::runtime_error {
    public:
        explicit GlazeError(const glz::parse_error &err,
            std::string_view buffer);
    };

}
