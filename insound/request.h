#pragma once

#include "json.h"


namespace Insound {

    /**
     * HttpMethod constants. Usable in the Insound::request function.
     */
    namespace HttpMethod {
        extern const std::string Get;
        extern const std::string Post;
        extern const std::string Put;
        extern const std::string Patch;
        extern const std::string Delete;
    }

    /**
     * Send an http request and receive body of the response.
     *
     * @param  url     end point url
     * @param  method  http method, use constants in Insound::HttpMethod namespace
     * @param  payload optional Json payload to set
     * @return         body of the response as a Json object
     */
    Poco::JSON::Object::Ptr request(const std::string &url, const std::string &method = HttpMethod::Get, const Json *payload = nullptr);

}
