#include "catch2/catch_test_macros.hpp"
#include <insound/tests/definitions.h>
#include <insound/tests/test.h>
#include <insound/core/BankBuilder.h>
#include <fstream>
#include <thread>

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

static std::string file1;
static std::string file2;
static std::string file3;

class BankTest {
public:
    BankTest()
    {
        REQUIRE(BankBuilder::initLibrary() == nullptr);
        file1 = openFile(PUBLIC_DIR "/audio/test.mp3");
        file2 = openFile(PUBLIC_DIR "/audio/test.ogg");
        file3 = openFile(PUBLIC_DIR "/audio/test.wav");
    }

    ~BankTest()
    {
        REQUIRE(BankBuilder::closeLibrary() == nullptr);
    }


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


void addToVector(std::vector<std::vector<uint8_t>> &files, const std::vector<uint8_t> &file)
{
    static std::mutex lock;
    lock.lock();

    files.emplace_back(file);

    lock.unlock();
}

TEST_CASE_METHOD(BankTest, "Bank can build in multithreaded context")
{
    const auto NumThreads = 100;
    std::vector<std::thread> threads;
    std::vector<std::vector<uint8_t>> files;

    // Build 100 banks simultaneously
    for (int i = 0; i < NumThreads; ++i)
    {
        threads.emplace_back([&files]() {
            BankBuilder bank;
            bank.addFile(file1.data(), file1.size());
            bank.addFile(file2.data(), file2.size());
            bank.addFile(file3.data(), file3.size());
            REQUIRE(bank.build() == nullptr);
            REQUIRE(!bank.data().empty());
            addToVector(files, bank.data());
        });
    }

    // await threads
    for (int i = 0; i < NumThreads; ++i)
    {
        threads[i].join();
    }

    REQUIRE(!files.empty());
    REQUIRE(files.size() == NumThreads);

    bool allFilesMatch = true;
    // check if collected data matches
    for (int i = 0; i < NumThreads; ++i)
    {
        if (files[0] != files[i])
        {
            allFilesMatch = false;
            break;
        }
    }

    REQUIRE(allFilesMatch);
}
