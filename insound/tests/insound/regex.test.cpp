#include <insound/tests/test.h>
#include <insound/core/regex.h>

TEST_CASE("REGEXP Email")
{
    REQUIRE(std::regex_match("hi@hello.com", Insound::Regex::email));
    REQUIRE(std::regex_match("hi@.com", Insound::Regex::email) == false);
    REQUIRE(std::regex_match("@hello.com", Insound::Regex::email) == false);
    REQUIRE(std::regex_match("1@1.com", Insound::Regex::email));
    REQUIRE(std::regex_match("@.com", Insound::Regex::email) == false);
    REQUIRE(std::regex_match("hi.com", Insound::Regex::email) == false);
    REQUIRE(std::regex_match(".com", Insound::Regex::email) == false);
    REQUIRE(std::regex_match(".", Insound::Regex::email) == false);
    REQUIRE(std::regex_match("", Insound::Regex::email) == false);
    REQUIRE(std::regex_match("h-i@h-ello.com", Insound::Regex::email));
    REQUIRE(std::regex_match("h.i@h.ello.com", Insound::Regex::email));
}
