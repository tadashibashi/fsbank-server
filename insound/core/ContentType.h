/**
 * @file ContentType.h
 *
 * Contains constants containing various mimetypes used by the application.
 * These constants should be used for setting values in responses rather than
 * deciphering content-type values from requests.
 */
#pragma once
#include <string_view>

namespace Insound::ContentType {


    namespace Application
    {
        extern const std::string_view JSON;

        // .bin, and other binary files
        extern const std::string_view OctetStream;

        [[nodiscard]]
        bool isApplication(std::string_view mime);
    }

    namespace Audio
    {
        extern const std::string_view AAC;
        extern const std::string_view ALAC;
        extern const std::string_view FLAC;
        extern const std::string_view MP3;
        extern const std::string_view OGG;
        extern const std::string_view WAV;

        /**
         * WAV has several different possible mime types. Use this to check
         * if a content-type header is a WAV.
         */
        [[nodiscard]]
        bool isWAV(std::string_view mime);

        [[nodiscard]]
        bool isAudio(std::string_view mime);
    }

    namespace Text
    {
        extern const std::string_view CSS;
        extern const std::string_view HTML;
        extern const std::string_view Plain;
        extern const std::string_view JavaScript;

        [[nodiscard]]
        bool isText(std::string_view mime);
    }

    namespace Image
    {
        extern const std::string_view BMP;
        extern const std::string_view JPG;
        extern const std::string_view PNG;
    }
}
