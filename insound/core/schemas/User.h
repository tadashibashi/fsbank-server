#pragma once
#include <insound/core/format.h>
#include <string>

namespace Insound {
    /**
     * User model stored in the database
     */
    class User {
    public:
        virtual ~User() = default;

        enum class Type : unsigned int {
            Guest, Unverified, User, Staff, Admin
        };

        /**
         * Username that can be used as a link to one's profile
         */
        std::string username;

        /**
         * Name that appears to other users in profile view, author name, etc.
         */
        std::string displayName;

        /**
         * User email address
         */
        std::string email;

        /**
         * Encrypted password, hidden from the frontend
         */
        std::string password;

        /**
         * A value from the Type struct representing user type. Get permission
         * level via Insound::User::getTypeLevel.
         */
        Type type = Type::Unverified;

        /**
         * Whether user is authorized at the level of `userType`.
         * User a string constant from Insound::User::Type.
         */
        [[nodiscard]]
        bool isAuthorized(Type userType = Type::User) const;

        /**
         * Whether user is authorized at the level of staff or higher.
         */
        [[nodiscard]]
        bool isStaff() const;

        [[nodiscard]]
        bool isAdmin() const;

        [[nodiscard]]
        bool isVerified() const;
    };

    /**
     * Token passed to the frontend
     */
    class UserToken : public User {
    public:
        /**
         * Encrypted fingerprint
         */
        std::string fingerprint;
    };
}
