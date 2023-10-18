#include <insound/env.h>
#include <insound/jwt.h>
#include <catch2/catch_test_macros.hpp>

#include <insound/chrono.h>

using namespace Insound::ChronoLiterals;

#include <insound/thirdparty/glaze.hpp>

struct person {
    std::string name;
    int age;
};
GLZ_META(person, name, age);


TEST_CASE ("JWT serialize & deserialize", "[Jwt]")
{
    // Configure the JWT_SECRET test variable
    Insound::configureEnv("tests/test-env");

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
