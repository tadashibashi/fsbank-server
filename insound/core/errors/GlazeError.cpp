#include "GlazeError.h"


namespace Insound {
    GlazeError::GlazeError(
        const glz::parse_error &err,
        std::string_view buffer) : std::runtime_error(
            sf("Glaze error, with code {}:\n{}", (int)err.ec,
                glz::format_error(err, buffer))
        ) {}
}
