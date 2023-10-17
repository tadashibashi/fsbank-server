#include "insound/env.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <cstring>

TEST_CASE("Read/configure environment variables from file", "[env]")
{
    SECTION("Test Variables do not exist in environment")
    {
        REQUIRE (std::getenv("APPLES") == nullptr);
        REQUIRE (Insound::getEnv("APPLES", "NULL") == "NULL");
    }

    SECTION("getEnv gets environment variable")
    {
        setenv ("TestValue", "1234567", true);
        REQUIRE (Insound::getEnv("TestValue") == "1234567");

        unsetenv ("TestValue");
        REQUIRE (Insound::getEnv("TestValue") == "");
    }

    SECTION("Test Variables are set when loaded")
    {
        Insound::configureEnv("tests/test-env");

        REQUIRE ( Insound::getEnv("APPLES") == "1" );
        REQUIRE ( Insound::getEnv("GRAPES") == "2" );
        REQUIRE ( Insound::getEnv("ORANGES") == "three" );

        REQUIRE ( Insound::getEnv("PIZZA") == "");
    }

    SECTION("Test Variables are unset via resetEnv")
    {
        Insound::configureEnv("tests/test-env");

        REQUIRE ( Insound::getEnv("APPLES") == "1" );

        Insound::resetEnv("tests/test-env");

        REQUIRE ( std::getenv("APPLES") == nullptr );
    }
}
