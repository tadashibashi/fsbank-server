#include "CurlHeaderError.h"
#include <curl/curl.h>

namespace Insound {
    static const char *GetHErrorString(int code) {
        switch (code) {
        case CURLHE_OK: return "OK";
        case CURLHE_BADINDEX: return "There is no header with the requested index.";
        case CURLHE_MISSING: return "No such header exists.";
        case CURLHE_NOHEADERS: return "No headers at all have been recorded.";
        case CURLHE_NOREQUEST: return "There was no such request number.";
        case CURLHE_OUT_OF_MEMORY: return "Out of resources.";
        case CURLHE_BAD_ARGUMENT: return "One or more of the given arguments are bad.";
        case CURLHE_NOT_BUILT_IN: return "HTTP support or the header API has been disabled in the build.";
        default:
            return "Unkown error.";
        }
    }

    CurlHeaderError::CurlHeaderError(int curlHError) : std::runtime_error(
        sf("Curl Header Error {}: {}", curlHError, GetHErrorString(curlHError))
    ) { }
}
