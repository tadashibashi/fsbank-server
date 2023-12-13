#pragma once

#include <map>
#include <string>
#include <vector>

namespace Insound
{
    class MixPreset
    {
    public:
        std::string name;
        std::vector<int> levels;
    };

    struct TrackChannel
    {
    };

    class Track
    {
    public:
        Track() = default;

        std::string title;

        // Whether or not this track should loop
        bool isLooping;

        // Sample position of loop start
        unsigned long long loopStart;
        // Sample position of loop end
        unsigned long long loopEnd;

        // name of each original file, used for retrieving and saving files
        std::vector<std::string> channels;

        // document ID of owner
        std::string owner;


        // ===== Helper functions =============================================

        /**
         * Download track with each original audio file bundled into a zip
         *
         * @return buffer containing raw binary data.
         */
        std::string downloadZip() const;
    };
}
