#include <insound/tests/test.h>
#include <insound/tests/env.h>
#include <insound/core/password.h>


TEST_CASE ("Test password hash & compare", "[password, hash]")
{
    Insound::configureEnv(ENV_FILEPATH);

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
