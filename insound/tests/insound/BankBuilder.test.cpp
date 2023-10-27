#include "catch2/catch_test_macros.hpp"
#include <insound/tests/definitions.h>
#include <insound/tests/test.h>
#include <insound/core/BankBuilder.h>
#include <fstream>

static std::string openFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file at path " + path);
    }

    file.seekg(0, std::ios::end);
    auto filesize = file.tellg();
    std::string buffer(filesize, ' ');
    file.seekg(0);
    file.read(buffer.data(), filesize);

    file.close();
    return buffer;
}



class BankTest {
public:
    BankTest() :
        file1(openFile(PUBLIC_DIR "/audio/test.mp3")),
        file2(openFile(PUBLIC_DIR "/audio/test.ogg")),
        file3(openFile(PUBLIC_DIR "/audio/test.wav"))
    {
        REQUIRE(BankBuilder::initLibrary() == nullptr);
    }

    ~BankTest()
    {
        REQUIRE(BankBuilder::closeLibrary() == nullptr);
    }

    std::string file1;
    std::string file2;
    std::string file3;
};

TEST_CASE_METHOD(BankTest, "Files have substance")
{
    REQUIRE(!file1.empty());
    REQUIRE(!file2.empty());
    REQUIRE(!file3.empty());
}

TEST_CASE_METHOD(BankTest, "Bank can build")
{
    BankBuilder bank;
    bank.addFile(file1.data(), file1.size());
    bank.addFile(file2.data(), file2.size());
    bank.addFile(file3.data(), file3.size());
    REQUIRE(bank.build() == nullptr);
    REQUIRE(!bank.data().empty());
}
