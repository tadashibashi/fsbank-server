#pragma once
#include <insound/thirdparty/glaze.hpp>
#include <string>

namespace Insound {
    class User {
    public:
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
         * Encrypted fingerprint
         */
        std::string fingerprint;

        /**
         * Encrypted password, hidden from the frontend
         */
        std::string password;

        /**
         * A value from the Type struct representing user type. Get permission
         * level via Insound::User::getTypeLevel.
         */
        std::string type;

        /**
         * Struct containing static constant strings enumerating user types.
         */
        struct Type {
            static const std::string Guest;
            static const std::string User;
            static const std::string Staff;
            static const std::string Admin;
        };

        /**
         * Whether user is authorized at the level of `userType`.
         * User a string constant from Insound::User::Type.
         */
        [[nodiscard]]
        bool isAuthorized(const std::string &userType = User::Type::User) const;

        /**
         * Whether user is authorized at the level of staff or higher.
         */
        [[nodiscard]]
        bool isStaff() const;

        [[nodiscard]]
        bool isAdmin() const;

        /**
         * Get the permission level number. Higher means higher permission
         * level.
         * @param  type - a string from Insound::User::Type
         * @return        Permission level.
         */
        static int getTypeLevel(const std::string &type);
    };
}

template<>
struct glz::meta<Insound::User> {
    using T = Insound::User;
    static constexpr auto value = glz::object(
        "username", &T::username,
        "displayName", &T::displayName,
        "email", &T::email,
        "type", &T::type,
        "password", glz::hide{&T::password}
    );
};
