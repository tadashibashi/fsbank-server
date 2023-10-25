#include <catch2/catch_session.hpp>

int main(int argc, char *argv[])
{
    // Global setup

    auto result = Catch::Session().run(argc, argv);

    // Tear down

    return result;
}
