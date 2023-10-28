#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/catch_config.hpp>
#include <insound/tests/env.h>
#include <insound/tests/test.h>
#include <insound/core/s3.h>

class S3Tests : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(const Catch::TestRunInfo &) override
    {
        configureEnv(ENV_FILEPATH);
        REQUIRE(S3::config());
        S3::dropBucket__permanent__(requireEnv("S3_BUCKET"));
        REQUIRE(S3::createBucket(requireEnv("S3_BUCKET")));
    }

    void testRunEnded(const Catch::TestRunStats &) override
    {
        S3::dropBucket__permanent__(requireEnv("S3_BUCKET"));
        S3::close();
    }
};
CATCH_REGISTER_LISTENER(S3Tests);

const auto FileContent = "hello world!";

TEST_CASE("S3::uploadFile uploads object string")
{
    REQUIRE(S3::uploadFile("test", FileContent));
}

TEST_CASE("S3::listObjects counts objects")
{
    auto list = S3::listObjects("test");
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == "test");
}

TEST_CASE("S3::downloadFile gets file")
{
    auto file = S3::downloadFile("test");
    REQUIRE(file);
    REQUIRE(file.value() == FileContent);
}

TEST_CASE("S3::deleteFile deletes file")
{
    REQUIRE(S3::deleteFile("test"));
    REQUIRE(!S3::downloadFile("test"));
}

TEST_CASE("S3::deleteFiles works")
{
    REQUIRE(S3::uploadFile("deleteFiles0", FileContent));
    REQUIRE(S3::uploadFile("deleteFiles1", FileContent));
    REQUIRE(S3::uploadFile("deleteFiles2", FileContent));
    REQUIRE(S3::listObjects("deleteFiles").size() == 3);

    REQUIRE(S3::deleteFiles({"deleteFiles0", "deleteFiles1", "deleteFiles2"}));

    REQUIRE(S3::listObjects("deleteFiles").empty());
}

TEST_CASE("S3::deleteFolder works")
{
    REQUIRE(S3::uploadFile("deleteFolder/file0", FileContent));
    REQUIRE(S3::uploadFile("deleteFolder/file1", FileContent));
    REQUIRE(S3::uploadFile("deleteFolder/file2", FileContent));
    REQUIRE(S3::listObjects("deleteFolder/").size() == 3);

    REQUIRE(S3::deleteFolder("deleteFolder"));
    REQUIRE(S3::listObjects("deleteFolder/").empty());
}

TEST_CASE("S3::zipFolder works")
{
    REQUIRE(S3::uploadFile("deleteFolder/file0", FileContent));
    REQUIRE(S3::uploadFile("deleteFolder/file1", FileContent));
    REQUIRE(S3::uploadFile("deleteFolder/file2", FileContent));
    REQUIRE(S3::listObjects("deleteFolder/").size() == 3);

    auto zip = S3::zipFolder("deleteFolder/");
    REQUIRE(zip);
    REQUIRE(!zip.value().empty());

    // TODO create ZipReader to check the contents of the binary
}
