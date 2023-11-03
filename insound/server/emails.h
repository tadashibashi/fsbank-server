/**
 * This file contains functions pertaining to Insound's automated emails,
 * such as for verification and other communications.
 */
#pragma once
#include <optional>
#include <string>
#include <string_view>

namespace Insound::Emails {

    /**
     * Returned from email text generator functions to return both a plain
     * text and html version of an email.
     */
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
     * Token to pass to email verification
     */
    struct EmailVerificationToken {
        std::string _id;
        std::string email;
    };

    /**
     * Create post-registration verification email message
     *
     * @param  email - the user's email address
     * @param  id    - the user's id
     *
     * @return email html and text forms
     */
    EmailStrings createVerificationStrings(std::string_view email,
        std::string_view id);


    /**
     * Check the verification token sent to the user via email.
     * If the token is valid, it will return
     *
     * @param  token - token to check
     *
     * @return         whether the token is valid
     */
    [[nodiscard]]
    std::optional<EmailVerificationToken> verifyEmail(std::string_view token);

}
