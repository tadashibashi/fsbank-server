#pragma once

#include <glaze/glaze.hpp>

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
     * Send a request, and receive the payload as a string
     * @param  url         endpoint url
     * @param  method      http method - use constants in Insound::HttpMethod namespace
     * @param  jsonPayload [optional] JSON payload string
     * @return             The JSON body as a string
     */
    std::string request(const std::string &url, const std::string &method = HttpMethod::Get, const std::string &jsonPayload = "");


    /**
     * Send an http request and receive body of the response.
     *
     * @param  url     endpoint url
     * @param  method  http method, use constants in Insound::HttpMethod namespace
     * @param  payload optional Json payload to set
     * @return         body of the response as a Json object
     *
     * @throws CurlError if an error occurred during the request
     */
    template <typename T, typename Payload = int>
    T request(const std::string &url, const std::string &method = HttpMethod::Get, const Payload *payload = nullptr)
    {
        auto stringified = payload ? glz::write_json(*payload) : std::string();
        auto body = request(url, method, stringified);

        T res;
        glz::context ctx{};
        glz::parse_error error = glz::read<glz::opts{
            .error_on_unknown_keys=false,
            .error_on_missing_keys=true
        }> (res, std::forward<std::string>(body), ctx);

        if (error.ec != glz::error_code::none)
            throw f("Glaze error: {}", (int)error.ec);

        return res;
    }
}
