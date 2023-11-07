#pragma once


#include <insound/core/HttpStatus.h>

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

        /**
         * Set the body to json data
         */
        template <typename T>
            requires glz::is_specialization_v<T, glz::meta> ||
            requires {T::glaze;}
        void json(const T &obj)
        {
            this->set_header("Content-Type", "application/json");
            this->body = glz::write_json(obj);
        }
    private:

    };
}
