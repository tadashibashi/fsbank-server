#include <insound/request.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Request sends get request")
{

    auto body = Insound::request("https://httpbin.org/ip",
        Insound::HttpMethod::Get);

    REQUIRE(!body->get("origin").isEmpty());
}

TEST_CASE ("Request sends and receives response body in post request")
{
    // Create the test object
    Insound::Json j;
    j.set("name", "joe");
    j.set("age", 10);

    // this route returns the posted data in a sub-object "json"
    auto body = Insound::request("https://httpbin.org/post",
        Insound::HttpMethod::Post, &j);

    auto json = body->getObject("json");

    // Ensure the returned data is as expected
    REQUIRE(json->get("name").toString() == "joe");
    REQUIRE(json->get("age").convert<int>() == 10);
}
