#include <insound/env.h>
#include <insound/password.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Test password hash & compare", "[password, hash]")
{
    Insound::configureEnv("tests/test-env");

    SECTION("Hashing password results in another string")
    {
        auto original = "password";
        auto hashed = Insound::hash(original);

        REQUIRE(original != hashed);
    }

    SECTION("Hashing password & comparing it works")
    {
        auto original = "password";
        auto hashed = Insound::hash(original);

        REQUIRE(Insound::compare(original, hashed));
    }
}
