#pragma once
#include <insound/core/schemas/User.h>
#include <insound/core/json.h>
#include <insound/core/thirdparty/glaze.hpp>

using Insound::User;
using Insound::UserToken;

template<>
struct glz::meta<User::Type> {
    using enum User::Type;

    // Once strings are set, don't change it, as they will be written this
    // way in the database and will become invalidated if changed.
    static constexpr auto value = glz::enumerate(
        "guest", Guest,
        "unverified", Unverified,
        "user", User,
        "staff", Staff,
        "admin", Admin
    );
};

template<>
struct glz::meta<User> {
    using T = User;

    // This allows Insound::User to be used as a mongo collection
    static constexpr std::string_view collection_name = "users";

    static constexpr auto value = glz::object(
        "username", &T::username,
        "displayName", &T::displayName,
        "email", &T::email,
        "type", &T::type,
        "password", &T::password
    );
};

template<>
struct glz::meta<UserToken> {
    using T = UserToken;
    static constexpr auto value = glz::object(
        "username", &T::username,
        "displayName", &T::displayName,
        "email", &T::email,
        "type", &T::type,
        "password", glz::hide{&T::password},
        "fingerprint", &T::fingerprint
    );
};

IN_JSON_FORMAT(User::Type);
