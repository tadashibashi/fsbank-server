#include <insound/thirdparty/glaze.hpp>
#include <string>

namespace Insound {
    struct User {
        std::string username;
        std::string email;
        std::string password;
        std::string type;
    };
}

template<>
struct glz::meta<Insound::User> {
    using T = Insound::User;
    static constexpr auto value = glz::object(
        "username", &T::username,
        "email", &T::email,
        "type", &T::type,
        "password", glz::hide{&T::password}
    );
};
