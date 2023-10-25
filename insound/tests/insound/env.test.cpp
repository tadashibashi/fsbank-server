#include <insound/tests/env.h>
#include <insound/tests/test.h>

#include <cstdlib>
#include <cstring>


TEST_CASE("getEnv reads from the environment", "[getEnv]")
{
    REQUIRE_NOTHROW(Insound::resetEnv(ENV_FILEPATH));
    REQUIRE (std::getenv("APPLES") == nullptr);
    REQUIRE (std::getenv("ORANGES") == nullptr);
    REQUIRE (std::getenv("GRAPES") == nullptr);

    SECTION("getEnv finds variable")
    {
        setenv ("TestValue", "1234567", true);
        REQUIRE (Insound::getEnv("TestValue") == "1234567");

        unsetenv ("TestValue");
        REQUIRE (Insound::getEnv("TestValue") == "");
    }

    SECTION("getEnv gets default value when non-existent variable queried")
    {
        unsetenv("TestValue");
        REQUIRE (Insound::getEnv("TestValue", "NULL") == "NULL");
    }
}

TEST_CASE("requireEnv reads from the environment", "[requireEnv]")
{
    REQUIRE_NOTHROW(Insound::resetEnv(ENV_FILEPATH));
    REQUIRE (std::getenv("APPLES") == nullptr);
    REQUIRE (std::getenv("ORANGES") == nullptr);
    REQUIRE (std::getenv("GRAPES") == nullptr);


    SECTION("requireEnv finds variable")
    {
        setenv ("TestValue", "1234567", true);
        REQUIRE (Insound::requireEnv("TestValue") == "1234567");
    }

    SECTION("requireEnv throws on missing variable")
    {
        REQUIRE_THROWS( Insound::requireEnv("ONIONS") );
    }
}

TEST_CASE("Read/configure environment variables from file", "[env]")
{
    REQUIRE_NOTHROW(Insound::resetEnv(ENV_FILEPATH));
    REQUIRE (std::getenv("APPLES") == nullptr);
    REQUIRE (std::getenv("ORANGES") == nullptr);
    REQUIRE (std::getenv("GRAPES") == nullptr);

    SECTION("Test Variables are set when loaded")
    {
        REQUIRE_NOTHROW(Insound::configureEnv(ENV_FILEPATH));

        REQUIRE ( Insound::getEnv("APPLES") == "1" );
        REQUIRE ( Insound::getEnv("GRAPES") == "2" );
        REQUIRE ( Insound::getEnv("ORANGES") == "three" );

        REQUIRE ( Insound::getEnv("PIZZA") == "");
    }

    SECTION("Test Variables are unset via resetEnv")
    {
        REQUIRE_NOTHROW(Insound::configureEnv(ENV_FILEPATH));

        REQUIRE ( Insound::getEnv("APPLES") == "1" );

        REQUIRE_NOTHROW(Insound::resetEnv(ENV_FILEPATH));

        REQUIRE ( std::getenv("APPLES") == nullptr );
    }
}
