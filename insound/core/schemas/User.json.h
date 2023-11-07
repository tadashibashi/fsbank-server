#pragma once
#include <insound/core/schemas/User.h>
#include <insound/core/thirdparty/glaze.hpp>

template<>
struct glz::meta<Insound::User::Type> {
    using enum Insound::User::Type;

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
struct glz::meta<Insound::User> {
    using T = Insound::User;

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
