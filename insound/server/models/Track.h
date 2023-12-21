#pragma once

#include <insound/core/schemas/User.json.h>
#include <insound/core/mongo/Document.h>

#include <map>
#include <string>
#include <vector>

namespace Insound
{
    struct MixPreset
    {
        std::string name;
        std::vector<double> levels;
    };

    struct TrackMarker
    {
        std::string text;
        double offset; // in seconds
    };

    struct TrackChannel
    {
        std::string name;     // user-provided layer name
        std::string filename; // randomly generated name indicating s3 filename
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

        // document ID of owner
        std::string owner;

        Mongo::Document<User> getOwner() const;

        std::vector<MixPreset> presets;

        std::vector<TrackChannel> channels;

        std::vector<TrackMarker> markers;


        // ===== Helper functions =============================================

        /**
         * Download track with each original audio file bundled into a zip
         *
         * @return buffer containing raw binary data.
         */
        std::string downloadZip() const;
    };
}
