#pragma once
#include <insound/core/format.h>
#include <insound/core/thirdparty/glaze.hpp>
#include <string>

namespace Insound {
    /**
     * User model stored in the database
     */
    class User {
    public:
        virtual ~User() = default;

        enum class Type : unsigned int {
            Guest, User, Staff, Admin
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
        Type type = Type::User;

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

// ----- Glaze Metadata ------------------------------------------------------

template<>
struct glz::meta<Insound::User::Type> {
    using enum Insound::User::Type;

    // Name must match precisely to enum, and once set, don't change it
    static constexpr auto value = glz::enumerate(
        "guest", Guest,
        "user", User,
        "staff", Staff,
        "admin", Admin
    );
};

template<>
struct glz::meta<Insound::User> {
    using T = Insound::User;
    static constexpr std::string_view name = "users";
    static constexpr auto value = glz::object(
        "username", &T::username,
        "displayName", &T::displayName,
        "email", &T::email,
        "type", &T::type,
        "password", &T::password
    );
};

template<>
struct glz::meta<Insound::UserToken> {
    using T = Insound::UserToken;
    static constexpr auto value = glz::object(
        "username", &T::username,
        "displayName", &T::displayName,
        "email", &T::email,
        "type", &T::type,
        "password", glz::hide{&T::password},
        "fingerprint", &T::fingerprint
    );
};

GLZ_FORMAT(Insound::User::Type);
