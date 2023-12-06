#include <insound/tests/test.h>
#include <insound/core/json.h>

#include <map>
#include <string>
#include <string_view>
#include <vector>

using namespace Insound;

TEST_CASE("Check serializing for non-glaze-specialized types")
{
    REQUIRE(JSON::stringify("Hello world") == "\"Hello world\"");
    REQUIRE(JSON::stringify(std::string_view("Hello world")) == "\"Hello world\"");
    REQUIRE(JSON::stringify(std::vector<int>{1, 2, 3, 4 ,5}) == "[1,2,3,4,5]");
    REQUIRE(JSON::stringify(std::list<int>{1, 2, 3, 4 ,5}) == "[1,2,3,4,5]");
    REQUIRE(JSON::stringify(std::map<std::string, int>{{"hello", 1}, {"world", 2}}) == R"({"hello":1,"world":2})");
}

struct TestStruct {
    std::string name;
    int value;
};

TEST_CASE("Check auto-serializing in newer Glaze versions")
{
    TestStruct param {
        .name="test-struct",
        .value=10
    };

    std::string_view expected = R"({"name":"test-struct","value":10})";

    REQUIRE(JSON::stringify(param) == expected);
}
