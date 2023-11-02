#pragma once
#include <crow/middleware.h>

#include <optional>
#include <string>

namespace Insound {

    /**
     * Middleware that injects important security headers into the application.
     * User may alter the headers in any middleware or route by setting the
     * Headers object in the context.
     *
     * All headers are set as the very last after_handle.
     */
    class Helmet : crow::ILocalMiddleware
    {
    public:
        using OptString = std::optional<std::string>;

        /**
         * Security headers to set. Please check implementation file for
         * defaults.
         *
         * For further reading on what these headers do please check out:
         * Helmet JS
         * - https://helmetjs.github.io
         * Article on Sentrium
         * - https://www.sentrium.co.uk/labs/application-security-101-http-headers
         *
         * Look up each individual header on MDN
         */
        struct Headers {
            OptString strictTransportSecurity;
            OptString contentSecurityPolicy;
            OptString crossOriginEmbedderPolicy;
            OptString crossOriginOpenerPolicy;
            OptString crossOriginResourcePolicy;
            OptString crossAgentCluster;
            OptString referrerPolicy;

            // Default: "no-sniff" prevents the browser from guessing the mime
            // type apart from what is specified in the Content-Type header.
            // Attackers can attempt to upload files that trick the browser
            // into thinking it is a different type and potentially execute
            // harmful code.
            OptString xContentTypeOptions;
            OptString xDnsPrefetchControl;
            OptString xDownloadOptions;

            // Indicate whether a browser should be allowed to render a page in
            // a `<frame>`, `<iframe>`, `<embed>`, or `<object>`
            // Prevent click-jacking by presenting our site's material on other
            // sites. "DENY" or "SAMEORIGIN"
            OptString xFrameOptions;

            // Limits data external resources, such as Adobe Flash and PDF
            // documents, can access on the domain. If you don't set this to
            // "none", other sites can embed this app's data in their content.
            OptString xPermittedCrossDomainPolicies;

            // On older browsers, uses faulty cross-site scripting protection
            // built-in to browsers that should be turned off "0", if possible
            OptString xXssProtection;
        };

        struct context
        {
            Headers headers;
            std::string nonce;
        };

        void before_handle(crow::request &req, crow::response &res,
            context &ctx);

        void after_handle(crow::request &req, crow::response &res,
                          context &ctx);
    };

}
