#pragma once

namespace Insound {

    /**
     * Http Status Codes
     *
     * (Add docs as research is done.)
     */
    enum class HttpStatus : int {
        // Everything so far is ok, and the client should continue with the
        // request or ignore it if it's already finished.
        Continue = 100,

        // Indicates a protocol to which the server switches. Server should
        // include an `Upgrade` response header to indicate which protocol it
        // switched to. E.g.
        // 101
        // Upgrade: websocket
        // Connection: Upgrade
        SwitchingProtocols = 101,

        // Request is OK without error
        OK = 200,

        // Success status, indicating that a resource has been created
        Created = 201,

        // Request accepted for processing, but not yet complete. 202 is non-
        // committal, since there is no way for the request to later send
        // a follow-up indicating any result.
        Accepted = 202,

        // Request was successful but some modification by a proxy has occurred
        // that differs from the main server's 200 OK-response.
        NonAuthoriativeInfo = 203,

        // Request was processed but, no content will be returned in body
        NoContent = 204,

        // Tells the client to reset the document view, to clear the content
        // e.g. a form, a canvas state, some other UI refresh, etc.
        ResetContent = 205,

        // The request succeeded, the body contains requested ranges of data,
        // as described in the Range header of the request. Check docs for
        // specific use cases (https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/206)
        PartialContent = 206,

        MultipleChoices = 300,
        MovedPermanently = 303,
        Found = 302,
        SeeOther = 303,
        NotModified = 304,
        TemporaryRedirect = 307,
        PermanentRedirect = 308,

        // Something was wrong with the client's request
        // e.g. bad params, body, headers, etc.
        BadRequest = 400,

        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        ProxyAuthRequired = 406,
        Conflict = 407,
        Gone = 408,
        PayloadTooLarge = 409,
        UnsupportedMediaType = 415,
        RangeNotSatisfiable = 416,
        ExpectationFailed = 417,
        PreconditionRequired = 428,
        TooManyRequests = 429,
        UnavailableForLegalReasons = 451,

        InternalServerError = 500,
        NotImplemented = 501,
        BadGateway = 502,
        ServiceUnavailable = 503,
        GatewayTimeout = 504,
        VariantAlsoNegotiates = 506,
    };
}
