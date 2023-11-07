#pragma once


#include <insound/core/HttpStatus.h>
#include <insound/core/json.h>

#include <crow/http_response.h>

#include <glaze/core/meta.hpp>
#include <glaze/json/write.hpp>
#include <glaze/util/type_traits.hpp>

#include <string>
#include <string_view>

namespace Insound {

    class Response : public crow::response
    {
    public:
        Response() : crow::response() {

        }

        Response(int code, std::string_view contentType, std::string_view body)
            : crow::response(code, contentType.data(), body.data())
            {
            }

        Response(std::string_view contentType, std::string_view body)
            : crow::response(contentType.data(), body.data())
        {
        }

        Response(const Response &other) : crow::response()
        {

        }

        /**
         * Set the body to json data
         */
        template <JSON::Serializable T>
        Response &json(const T &obj)
        {
            this->set_header("Content-Type", "application/json");
            this->body = glz::write_json(obj);

            return *this;
        }

        /**
         * Set both the content-type & body at one time
         *
         * @param  type - content type of body
         * @param  body - body to set
         *
         * @return      reference to this object for method chaining.
         */
        Response &content(std::string_view type, std::string_view body)
        {
            this->set_header("Content-Type", type.data());
            this->body = body;

            return *this;
        }
    private:

    };
}
