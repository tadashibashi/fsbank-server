#include <insound/tests/env.h>
#include <insound/tests/test.h>
#include <insound/core/s3.h>

class S3Tests {
protected:
    S3Tests() {
        configureEnv(ENV_FILEPATH);
        REQUIRE(S3::config());
    }
    ~S3Tests() {
        S3::close();
    }
};

const auto FileContent = "hello world!";

TEST_CASE_METHOD(S3Tests, "S3::uploadFile uploads object string")
{
    REQUIRE(S3::uploadFile("test", FileContent));
}

TEST_CASE_METHOD(S3Tests, "S3::listObjects counts objects")
{
    auto list = S3::listObjects("test");
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == "test");
}

TEST_CASE_METHOD(S3Tests, "S3::downloadFile gets file")
{
    auto file = S3::downloadFile("test");
    REQUIRE(file);
    REQUIRE(file.value() == FileContent);
}

TEST_CASE_METHOD(S3Tests, "S3::deleteFile deletes file")
{
    REQUIRE(S3::deleteFile("test"));
    REQUIRE(!S3::downloadFile("test"));
}
