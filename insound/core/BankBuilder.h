/**
 * @file BankBuilder.h
 *
 * Contains class for builder fsbanks from audio files.
 */
#pragma once

#include <vector>
#include <cstdint>

namespace Insound
{
    class BankBuilder
    {
    public:
        using Result = const char *;
        static const Result OK;

        BankBuilder();
        ~BankBuilder();

        static Result initLibrary() noexcept;
        static Result closeLibrary() noexcept;

        /**
         * Add a file pointer to the bank. Do not delete the file until you are
         * done with Bank functionality, as it is not copied.
         *
         * @param file      the file pointer to save with the bank.
         */
        Result addFile(void *data, unsigned size) noexcept;

        /**
         * Compile the bank into an fsbank, saving it at the location.
         *
         * A successful call to Bank::initLibrary must be made before running
         * this function.
         *
         * Make sure to consume the file via Bank::getLength and Bank::getFile
         * right after this call, since the next call to any other Bank::build
         * will overwrite it.
         *
         * @param samplerate    the desired samplerate, default: 44100
         *
         */
        Result build(float samplerate = 44100.f) noexcept;

        /**
         * Clear internals for object reuse.
         */
        Result clear() noexcept;

        /**
         * Get the data of the built file. Only available after a successful
         * call to BankBuilder::build.
         */
        [[nodiscard]]
        const std::vector<uint8_t> &data() const noexcept { return builtFile; }

    private:
        // Byte length of each file
        std::vector<unsigned> fileSizes;

        // File pointers
        std::vector<void *> files;

        // Data of the built file, only available after a successful call to
        // BankBuilder::build.
        std::vector<uint8_t> builtFile;
    };

}
