#include <insound/schemas/User.h>

#include <catch2/catch_test_macros.hpp>


using namespace Insound;

TEST_CASE ("User types are unique")
{
    REQUIRE(User::Type::Guest != User::Type::User);
    REQUIRE(User::Type::Guest != User::Type::Staff);
    REQUIRE(User::Type::Guest != User::Type::Admin);
    REQUIRE(User::Type::User != User::Type::Staff);
    REQUIRE(User::Type::User != User::Type::Admin);
    REQUIRE(User::Type::Staff != User::Type::Admin);
}

TEST_CASE ("User getTypeLevel returns the same level with same input")
{
    REQUIRE(User::getTypeLevel(User::Type::Guest) == User::getTypeLevel(User::Type::Guest));
    REQUIRE(User::getTypeLevel(User::Type::User) == User::getTypeLevel(User::Type::User));
    REQUIRE(User::getTypeLevel(User::Type::Staff) == User::getTypeLevel(User::Type::Staff));
    REQUIRE(User::getTypeLevel(User::Type::Admin) == User::getTypeLevel(User::Type::Admin));
}

TEST_CASE ("User levels are unique")
{
    REQUIRE(User::getTypeLevel(User::Type::Guest) != User::getTypeLevel(User::Type::User));
    REQUIRE(User::getTypeLevel(User::Type::Guest) != User::getTypeLevel(User::Type::Staff));
    REQUIRE(User::getTypeLevel(User::Type::Guest) != User::getTypeLevel(User::Type::Admin));
    REQUIRE(User::getTypeLevel(User::Type::User) != User::getTypeLevel(User::Type::Staff));
    REQUIRE(User::getTypeLevel(User::Type::User) != User::getTypeLevel(User::Type::Admin));
    REQUIRE(User::getTypeLevel(User::Type::Staff) != User::getTypeLevel(User::Type::Admin));
}

TEST_CASE ("User levels are in expected order")
{
    REQUIRE(User::getTypeLevel(User::Type::Guest) < User::getTypeLevel(User::Type::User));
    REQUIRE(User::getTypeLevel(User::Type::User) < User::getTypeLevel(User::Type::Staff));
    REQUIRE(User::getTypeLevel(User::Type::Staff) < User::getTypeLevel(User::Type::Admin));
}
