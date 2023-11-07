#include "ContentType.h"

namespace Insound::ContentType {

    auto &npos = std::string_view::npos;

    namespace Application
    {
        const std::string_view JSON = "application/json";
        const std::string_view OctetStream = "application/octet-stream";

        bool isApplication(std::string_view mime)
        {
            return mime.starts_with("application/");
        }
    }

    namespace Audio
    {
        const std::string_view AAC = "audio/aac";
        const std::string_view ALAC = "audio/m4a";
        const std::string_view FLAC = "audio/flac";
        const std::string_view MP3 = "audio/mpeg";
        const std::string_view OGG = "audio/ogg";
        const std::string_view OPUS = "audio/opus";
        const std::string_view WAV = "audio/wav";

        bool isWAV(std::string_view mime)
        {
            return mime.starts_with("audio/") &&
                mime.find("wav", 6) != npos; // 6 is the length of "audio/"
        }

        bool isAudio(std::string_view mime)
        {
            return mime.starts_with("audio/");
        }
    }

    namespace Text
    {

        const std::string_view CSS = "text/css";
        const std::string_view HTML = "text/html";
        const std::string_view Plain = "text/plain";
        const std::string_view JavaScript = "text/javascript";

        bool isText(std::string_view mime)
        {
            return mime.starts_with("text/");
        }
    }

    namespace Image
    {
        const std::string_view BMP = "image/bmp";
        const std::string_view JPG = "image/jpeg";
        const std::string_view PNG = "image/png";
    }
}
