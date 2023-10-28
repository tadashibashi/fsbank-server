#include <insound/core/BankBuilder.h>
#include <insound/tests/definitions.h>
#include <insound/tests/test.h>

#include <fstream>
#include <thread>


// ===== Helper function declarations =========================================

/**
 * Helper function for this test - opens binary files into strings
 *
 * @param  path - path to the file
 *
 * @returns     the file data wrapped in a string
 */
static std::string openFile(const std::string &path);

/**
 * Thread-safe helper to add a file string to a files vector.
 *
 * @param files - files list
 * @param file  - file to add
 */
static void addToVector(std::vector<std::vector<uint8_t>> &files,
    const std::vector<uint8_t> &file);


// ===== Tests ================================================================

class BankBuilderTest
{
public:
    // Test file data
    inline static std::string file1{};
    inline static std::string file2{};
    inline static std::string file3{};

    // Setup
    BankBuilderTest()
    {
        REQUIRE(BankBuilder::initLibrary() == nullptr);
        file1 = openFile(PUBLIC_DIR "/audio/test.mp3");
        file2 = openFile(PUBLIC_DIR "/audio/test.ogg");
        file3 = openFile(PUBLIC_DIR "/audio/test.wav");
    }

    // Tear down
    ~BankBuilderTest()
    {
        REQUIRE(BankBuilder::closeLibrary() == nullptr);
        file1.clear();
        file2.clear();
        file3.clear();
    }
};


TEST_CASE_METHOD(BankBuilderTest, "Files have substance")
{
    REQUIRE(!file1.empty());
    REQUIRE(!file2.empty());
    REQUIRE(!file3.empty());
}


TEST_CASE_METHOD(BankBuilderTest, "Bank can build")
{
    BankBuilder bank;
    bank.addFile(file1.data(), file1.size());
    bank.addFile(file2.data(), file2.size());
    bank.addFile(file3.data(), file3.size());
    REQUIRE(bank.build() == nullptr);
    REQUIRE(!bank.data().empty());
}


TEST_CASE_METHOD(BankBuilderTest, "Bank can build in multithreaded context")
{
    const auto NumThreads = 100;
    std::vector<std::thread> threads;
    std::vector<std::vector<uint8_t>> files;

    // Build 100 banks in 100 threads
    for (int i = 0; i < NumThreads; ++i)
    {
        threads.emplace_back(
            [&files]()
            {
                // create bank, add files
                BankBuilder bank;
                bank.addFile(file1.data(), file1.size());
                bank.addFile(file2.data(), file2.size());
                bank.addFile(file3.data(), file3.size());

                // build
                REQUIRE(bank.build() == nullptr);
                REQUIRE(!bank.data().empty());

                // collect files
                addToVector(files, bank.data());
            }
        );
    }

    // await threads
    for (int i = 0; i < NumThreads; ++i)
    {
        threads[i].join();
    }

    // check collected files
    REQUIRE(!files.empty());
    REQUIRE(files.size() == NumThreads);

    bool allFilesMatch = true;
    for (int i = 1; i < NumThreads; ++i)
    {
        if (files[0] != files[i])
        {
            allFilesMatch = false;
            break;
        }
    }

    REQUIRE(allFilesMatch);
}


// ===== Helper function definitions ==========================================

std::string openFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::binary);
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

void addToVector(std::vector<std::vector<uint8_t>> &files,
    const std::vector<uint8_t> &file)
{
    static std::mutex lock;
    lock.lock();

    files.emplace_back(file);

    lock.unlock();
}
