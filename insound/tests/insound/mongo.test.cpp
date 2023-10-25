#include <insound/tests/test.h>
#include <insound/core/mongo/Model.h>
#include <insound/core/mongo.h>

#include <insound/tests/env.h>

#include <glaze/core/macros.hpp>

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
    configureEnv(ENV_FILEPATH);
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

        auto &doc = result.value();
        REQUIRE (doc.body.name == "Bob");
        REQUIRE (doc.body.age == 30);

        result = PersonModel.findOne({"name", "Bob"});

        REQUIRE(result);
        auto &doc2 = result.value();

        REQUIRE (doc.body.name == "Bob");
        REQUIRE (doc.body.age == 30);

        auto delResult = PersonModel.deleteOne(
            {"_id", doc.id.oid().value()}
        );

        REQUIRE(delResult);
    }

}
