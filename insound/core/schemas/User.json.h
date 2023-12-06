#pragma once
#include <insound/core/schemas/User.h>
#include <insound/core/json.h>

using Insound::User;
using Insound::UserToken;

IN_JSON_ENUM(User::Type, Guest, Unverified, User, Staff, Admin);
IN_JSON_META(User, username, displayName, email, type, password);

// explicit template to hide password
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
