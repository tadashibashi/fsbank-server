#include <insound/env.h>
#include <insound/jwt.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("JWT serialize & deserialize", "[Jwt]")
{
    // Configure the JWT_SECRET test variable
    Insound::configureEnv("tests/test-env");

    SECTION("sign does not throw & generates token string")
    {
        Poco::JSON::Object json;
        json.set("name", "Joe");
        json.set("age", 42);

        std::string jwt;
        REQUIRE_NOTHROW(jwt = Insound::Jwt::serialize(json));

        REQUIRE(!jwt.empty());

        Poco::JSON::Object res;
        REQUIRE_NOTHROW(res = Insound::Jwt::deserialize(jwt));

        REQUIRE (json.get("name").toString() == res.get("name").toString());
        REQUIRE (json.get("age").convert<int>() == res.get("age").convert<int>());
    }

}
