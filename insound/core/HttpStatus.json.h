#pragma once
#include "HttpStatus.h"

#include <insound/core/thirdparty/glaze.hpp>

template<>
struct glz::meta<Insound::HttpStatus> {
    using enum Insound::HttpStatus;

    static constexpr auto value = glz::enumerate(
        "100 Continue", Continue,
        "101 Switching Protocols", SwitchingProtocols,
        "200 OK", OK,
        "201 Created", Created,
        "202 Accepted", Accepted,
        "203 Non-Authoritative Information", NonAuthoriativeInfo,
        "204 No Content", NoContent,
        "205 Reset Content", ResetContent,
        "206 Partial Content", PartialContent,
        "300 Multiple Choices", MultipleChoices,
        "301 Moved Permanently", MovedPermanently,
        "302 Found", Found,
        "303 See Other", SeeOther,
        "304 Not Modified", NotModified,
        "307 Temporary Redirect", TemporaryRedirect,
        "308 Permanent Redirect", PermanentRedirect,
        "400 Bad Request", BadRequest,
        "401 Unauthorized", Unauthorized,
        "403 Forbidden", Forbidden,
        "404 Not Found", NotFound,
        "405 Method Not Allowed", MethodNotAllowed,
        "407 Proxy Authentication Required", ProxyAuthRequired,
        "409 Conflict", Conflict,
        "410 Gone", Gone,
        "413 Payload Too Large", PayloadTooLarge,
        "415 Unsupported Media Type", UnsupportedMediaType,
        "416 Range Not Satisfiable", RangeNotSatisfiable,
        "417 Expectation Failed", ExpectationFailed,
        "428 Precondition Required", PreconditionRequired,
        "429 Too Many Requests", TooManyRequests,
        "451 Unavailable For Legal Reasons", UnavailableForLegalReasons,
        "500 Internal Server Error", InternalServerError,
        "501 Not Implemented", NotImplemented,
        "502 Bad Gateway", BadGateway,
        "503 Service Unavailable", ServiceUnavailable,
        "504 Gateway Timeout", GatewayTimeout,
        "506 Variant Also Negotiates", VariantAlsoNegotiates
    );
};
