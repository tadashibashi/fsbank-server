#include <insound/schemas/User.h>

#include <catch2/catch_test_macros.hpp>


using namespace Insound;

TEST_CASE ("User levels are in expected order")
{
    REQUIRE(User::Type::Guest < User::Type::User);
    REQUIRE(User::Type::User < User::Type::Staff);
    REQUIRE(User::Type::Staff < User::Type::Admin);
}
