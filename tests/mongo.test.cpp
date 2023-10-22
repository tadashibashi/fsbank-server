#include "insound/env.h"
#include <glaze/core/macros.hpp>
#include <insound/models/Model.h>
#include <insound/mongo.h>
#include <catch2/catch_test_macros.hpp>

using namespace Insound;

struct Person {
    std::string name;
    int age;
};

template<>
struct glz::meta<Person> {
    using T = Person;
    static constexpr std::string_view name = "people";
    static constexpr auto value = glz::object(
        "name", &T::name,
        "age", &T::age);
};

TEST_CASE ("Mongodb tests")
{
    configureEnv("tests/test-env");
    auto connected = Mongo::connect();
    REQUIRE(connected);

    SECTION ("Mongo Document")
    {
        Mongo::Model<Person> PersonModel;
        auto result = PersonModel.insertOne({
            .name = "Bob",
            .age = 30
        });
        REQUIRE (result);
        REQUIRE (result.value().body.name == "Bob");
        REQUIRE (result.value().body.age == 30);

        result = PersonModel.findOne({"name", "Bob"});
        REQUIRE (result);
        REQUIRE (result.value().body.name == "Bob");
        REQUIRE (result.value().body.age == 30);

        IN_LOG(result.value().id);

        auto delResult = PersonModel.deleteOne({"_id", bsoncxx::oid{result.value().id} });
        REQUIRE(delResult);
    }

}
