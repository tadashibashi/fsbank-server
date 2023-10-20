#include "BankBuilder.h"

#include <insound/thirdparty/fsbank.hpp>

#include <cassert>
#include <thread>

static const char *CacheDirectory = ".fscache";

namespace Insound {
    // Build Vorbis when available. (Not available on ARM64 Macs)
    #if defined(INSOUND_PLATFORM_MAC) && defined (INSOUND_CPU_ARM64)
        static const FSBANK_FORMAT BankFormat = FSBANK_FORMAT_FADPCM;
    #else
        static const FSBANK_FORMAT BankFormat = FSBANK_FORMAT_VORBIS;
    #endif

    static bool wasInit;

    const BankBuilder::Result BankBuilder::OK = nullptr;

    BankBuilder::BankBuilder()
    {

    }

    BankBuilder::~BankBuilder()
    {
    }

    BankBuilder::Result BankBuilder::initLibrary() noexcept
    {
        if (!wasInit)
        {
            const auto NumCores = std::thread::hardware_concurrency();
            FSB_CHECK(
                FSBank_Init(
                    FSBANK_FSBVERSION_FSB5,
                    FSBANK_INIT_DONTLOADCACHEFILES,
                    NumCores,
                    CacheDirectory)
            );

            wasInit = true;
        }

        return OK;
    }

    BankBuilder::Result BankBuilder::closeLibrary() noexcept
    {
        if (wasInit)
        {
            FSB_CHECK( FSBank_Release() );
            wasInit = false;
        }

        return OK;
    }

    BankBuilder::Result BankBuilder::addFile(void *file, unsigned byteLength) noexcept
    {
        try {
            // ensure file is not null or empty
            if (file == nullptr)
                return "file pointer must not be null";
            if (byteLength == 0)
                return "byteLength must not be 0";

            files.emplace_back(file);
            fileSizes.emplace_back(byteLength);

            return OK;
        } catch (const std::exception &e) {
            return e.what();
        } catch (...) {
            return "an unknown error occured";
        }
    }

    BankBuilder::Result BankBuilder::build(float samplerate) noexcept
    {
        try {
            if (files.size() != fileSizes.size())
                return "files and fileSizes mismatch length";

            // Prepare subsound to build
            auto subsound = FSBANK_SUBSOUND();
            subsound.desiredSampleRate = samplerate;
            subsound.numFiles = files.size();
            subsound.fileData = files.data();
            subsound.fileDataLengths = fileSizes.data();

            // Run build
            FSB_CHECK( FSBank_Build(&subsound, 1, BankFormat, FSBANK_BUILD_NOGUID, 100, nullptr, nullptr) );

            // Get file data
            const void *builtFilePtr;
            unsigned int builtFileSize;
            FSB_CHECK( FSBank_FetchFSBMemory(&builtFilePtr, &builtFileSize) );

            auto tempBuiltFile = std::vector<uint8_t>((uint8_t *)builtFilePtr, (uint8_t *)builtFilePtr + builtFileSize);

            // Done, commit changes
            builtFile.swap(tempBuiltFile);
            return OK;
        } catch (const std::exception &e) {
            return e.what();
        } catch (...) {
            return "an unknown error occurred";
        }
    }

    BankBuilder::Result BankBuilder::clear() noexcept
    {
        try {
            builtFile.clear();
            files.clear();
            fileSizes.clear();
            return OK;
        } catch (const std::exception &e) {
            return e.what();
        } catch (...) {
            return "an unknown error occurred";
        }
    }
}
