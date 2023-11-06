#include <insound/core/schemas/User.json.h>

#include <catch2/catch_test_macros.hpp>
#include <glaze/glaze.hpp>

using namespace Insound;

TEST_CASE ("User type levels are in expected order")
{
    REQUIRE(User::Type::Guest < User::Type::User);
    REQUIRE(User::Type::User < User::Type::Staff);
    REQUIRE(User::Type::Staff < User::Type::Admin);
}

TEST_CASE ("User type level enums are spelled correctly")
{
    REQUIRE(glz::write_json(User::Type::Guest) == "\"guest\"");
    REQUIRE(glz::write_json(User::Type::User) == "\"user\"");
    REQUIRE(glz::write_json(User::Type::Staff) == "\"staff\"");
    REQUIRE(glz::write_json(User::Type::Admin) == "\"admin\"");
}
