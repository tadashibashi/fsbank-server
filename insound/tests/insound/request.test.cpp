#include <insound/tests/test.h>
#include <insound/core/request.h>

#include <insound/core/thirdparty/glaze.hpp>

// ===== JSON Schemas =========================================================

struct ip_response {
    std::string origin;
};

struct person_data {
    std::string name;
    int age;
};

struct person_response {
    person_data json;
};

GLZ_META(ip_response, origin);
GLZ_META(person_data, name, age);
GLZ_META(person_response, json);


// ===== Tests ================================================================

TEST_CASE ("Request sends get request")
{
    auto body = Insound::request<ip_response>("https://httpbin.org/ip",
        Insound::HttpMethod::Get);

    REQUIRE(!body.origin.empty());
}

TEST_CASE ("Request sends and receives response body in post request")
{
    // Create the test object
    person_data p {
        .name = "Joe",
        .age = 10,
    };

    // this route returns the posted data in a sub-object "json"
    auto body = Insound::request<person_response>("https://httpbin.org/post",
        Insound::HttpMethod::Post, &p);

    // Ensure the returned data is as expected
    REQUIRE(body.json.name == "Joe");
    REQUIRE(body.json.age == 10);
}

TEST_CASE ("Request throws when data does not meet expected type")
{
    ip_response p {
        .origin = "0.0.0.0",
    };

    person_response body;
    REQUIRE_THROWS(body = Insound::request<person_response>("https://httpbin.org/post",
        Insound::HttpMethod::Post, &p));
}
