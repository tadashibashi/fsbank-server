#include <insound/tests/test.h>
#include <insound/tests/env.h>

#include <insound/core/jwt.h>


// Use insound's chrono literals
#include <insound/core/chrono.h>
using namespace Insound::ChronoLiterals;

#include <insound/core/thirdparty/glaze.hpp>

struct person {
    std::string name;
    int age;
};
GLZ_META(person, name, age);


TEST_CASE ("JWT serialize & deserialize", "[Jwt]")
{
    // Configure the JWT_SECRET test variable
    Insound::configureEnv(ENV_FILEPATH);

    SECTION("sign does not throw & generates token string")
    {
        person p {
            .name = "Joe",
            .age = 42,
        };

        std::string jwt;
        REQUIRE_NOTHROW(jwt = Insound::Jwt::sign(p, 14_d)); // 14 day expiry

        REQUIRE(!jwt.empty());

        person res;
        REQUIRE_NOTHROW(res = Insound::Jwt::verify<person>(jwt));

        REQUIRE (p.name == res.name);
        REQUIRE (p.age == res.age);
    }

    SECTION("expired token throws")
    {
        person p {
            .name = "Bob",
            .age = 43,
        };

        std::string jwt;
        REQUIRE_NOTHROW(jwt = Insound::Jwt::sign(p, -1_s));

        REQUIRE(!jwt.empty());

        REQUIRE_THROWS(Insound::Jwt::verify(jwt));
    }

}
