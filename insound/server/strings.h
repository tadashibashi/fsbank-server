#pragma once
#include <insound/core/schemas/User.h>

namespace Insound::Emails {

    struct EmailStrings {
        /**
         * HTML version
         */
        std::string html;
        /**
         * Plain-text version, just in case
         */
        std::string text;
    };

    /**
     * Create verification email for registration
     * @param  token - User token to serialize
     */
    EmailStrings verification(const Insound::UserToken &token);

}
