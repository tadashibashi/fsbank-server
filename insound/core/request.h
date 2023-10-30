#pragma once

#include <insound/core/errors/GlazeError.h>
#include <glaze/glaze.hpp>

#include <string>
#include <string_view>
#include <vector>

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

    template <typename T = std::string>
    struct CurlResponse {
        long code;
        T body;
    };

    /**
     * Class for creating http requests
     */
    class MakeRequest
    {
    public:
        MakeRequest();
        explicit MakeRequest(const std::string_view &url,
            const std::string_view &method = "GET");
        ~MakeRequest();

        MakeRequest &url(const std::string_view &url);

        /**
         * Set the http method e.g. "GET", "POST", "PATCH", "PUT", "DELETE"
         *
         * @param  method - method to set
         *
         * @return          a reference to this object for method chaining.
         */
        MakeRequest &method(const std::string_view &method);


        /**
         * Set an http header
         *
         * @param  name  - header name to set
         * @param  value - value to set the header
         *
         * @return         a reference to this object for method chaining.
         */
        MakeRequest &header(const std::string_view &name,
            const std::string_view &value);


        /**
         * Send/perform the request, converting json response body to data type
         * specified - type `T` must specialize the glz::meta class
         *
         * @return object of type `T` parsed from the body
         *
         * @throws if there was a problem with the request, or if there was
         *         a problem parsing the response body.
         */
        template <typename T>
        T send()
        {
            T obj;
            glz::read<glz::opts{
                .error_on_unknown_keys=false,
                .error_on_missing_keys=false
            }>(send(), obj);

            return obj;
        }


        /**
         * Send/perform the request, retrieving the JSON response body as
         * a string.
         */
        std::string send();


        /**
         * Get the first header with `name` in response retrieved after a call
         * to `MakeRequest::send`. This function is only valid after
         * a successful call to send.
         *
         * @param  name - name of the header to find
         *
         * @return        value of the found header
         *
         * @throws CurlHeaderError if an error occurred. This includes not
         *         finding a header with `name`.
         */
        std::string getHeader(const std::string_view &name) const;

        /**
         * Get all headers with the indicated `name` in response retrieved
         * after a call to `MakeRequest::send`. Sometimes there are
         * multiple headers with the same name. This function is for those
         * cases. If only one exists, the vector will be one in size.
         *
         * This function is only valid after a successful call to send.
         *
         * @param name - name of the header group
         *
         * @return header values
         */
        std::vector<std::string> getHeaders(const std::string_view &name) const;

        /**
         * Get http response code after a call to `MakeRequest::send`. This
         * function is only valid after a successful call to `send`.
         *
         * @return http response code
         */
        long getCode() const;

        /**
         * Resets the object with a new request.
         */
        void clear();

        MakeRequest &body(const std::string_view &payload);
    private:
        struct Impl;
        Impl *m;
    };


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

        if (error)
            throw GlazeError(error, body);

        return res;
    }
}
