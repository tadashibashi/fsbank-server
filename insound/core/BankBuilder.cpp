#include "BankBuilder.h"
#include "fsbank.h"
#include "insound/core/definitions.h"

#include <insound/core/platform.h>
#include <insound/core/thirdparty/fsbank.hpp>
#include <insound/core/util.h>

#include <cassert>
#include <filesystem>
#include <fstream>
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

    static std::mutex build_lock;


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
        build_lock.lock();
        try {
            if (files.size() != fileSizes.size())
                return "files and fileSizes mismatch length";

            std::vector<FSBANK_SUBSOUND> subsounds;

            // Add files in reverse order, since indexes are reversed when
            // indexing the bank object in FMOD.
            for (int i = (int)files.size()-1; i >= 0; --i)
            {
                auto &subsound = subsounds.emplace_back();
                std::memset(&subsound, 0, sizeof(FSBANK_SUBSOUND));

                subsound.desiredSampleRate = samplerate;
                subsound.numFiles = 1;
                subsound.fileData = &files[i];
                subsound.fileDataLengths = &fileSizes[i];
            }

            // Run build
            FSB_CHECK( FSBank_Build(subsounds.data(), subsounds.size(), BankFormat,
                FSBANK_BUILD_DEFAULT, 75, nullptr, nullptr) );

            const void *data;
            unsigned int size;
            FSB_CHECK( FSBank_FetchFSBMemory(&data, &size) );

            auto retrieved = std::string((const char *)data,
                (const char *)data + size);

            // Done, commit changes
            builtFile.swap(retrieved);
            build_lock.unlock();
            return OK;
        }
        catch (const std::exception &e)
        {
            build_lock.unlock();
            return e.what();
        }
        catch (...) {
            build_lock.unlock();
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
        }
        catch (const std::exception &e)
        {
            return e.what();
        }
        catch (...)
        {
            return "an unknown error occurred";
        }
    }
}
