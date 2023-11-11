#include "Helmet.h"
#include <insound/core/env.h>
#include <insound/core/util.h>

void Insound::Helmet::before_handle(crow::request &req,
    crow::response &res, context &ctx)
{
    // Create nonce
    auto nonce = genBytes(16);
    auto nonceStr = crow::utility::base64encode(nonce.data(), nonce.size());
    ctx.nonce = nonceStr;
}

void Insound::Helmet::after_handle(crow::request &req, crow::response &res,
    context &ctx)
{
    static bool USE_SSL = requireEnv("HOST_ADDRESS").starts_with("https");

    auto &headers = ctx.headers;

    // set all headers
    res.add_header("Strict-Transport-Security",
        headers.strictTransportSecurity.value_or(
            "max-age=15552000; includeSubDomains"));

    res.add_header("Cross-Origin-Embedder-Policy",
        headers.crossOriginEmbedderPolicy.value_or("require-corp"));

    res.add_header("Cross-Origin-Opener-Policy",
        headers.crossOriginOpenerPolicy.value_or("same-origin"));

    res.add_header("Cross-Origin-Resource-Policy",
        headers.crossOriginResourcePolicy.value_or("same-origin"));

    res.add_header("Cross-Agent-Cluster",
        headers.crossAgentCluster.value_or("?1"));

    res.add_header("Referrer-Policy",
        headers.referrerPolicy.value_or("no-referrer"));

    res.add_header("X-Content-Type-Options",
        headers.xContentTypeOptions.value_or("no-sniff"));

    res.add_header("X-DNS-Prefetch-Control",
        headers.xDnsPrefetchControl.value_or("off"));


    res.add_header("X-Download-Options",
        headers.xDownloadOptions.value_or("noopen"));

    res.add_header("X-Frame-Options",
        headers.xFrameOptions.value_or("DENY"));

    res.add_header("X-Permitted-Cross-Domain-Policies",
        headers.xPermittedCrossDomainPolicies.value_or("none"));

    res.add_header("X-XSS-Protection",
        headers.xXssProtection.value_or("0"));

    auto &csp = headers.contentSecurityPolicy;

    // Add CSP
    res.add_header("Content-Security-Policy",
        f(
            "base-uri {0};"
            "font-src {1};"
            "form-action {2};"
            "frame-ancestors {3};"
            "img-src {4};"
            "object-src {5};"
            "script-src {6};"
            "style-src {7}"
            "{8}"
            ,
            csp.baseURI.value_or("'self'"),
            csp.fontSrc.value_or("'self'"),
            csp.formAction.value_or("'self'"),
            csp.frameAncestors.value_or("'none'"),
            csp.imgSrc.value_or("'self'"),
            csp.objectSrc.value_or("'none'"),
            csp.scriptSrc.value_or(f("'nonce-{}'", ctx.nonce)),
            csp.styleSrc.value_or(f("'nonce-{}'", ctx.nonce)),
            csp.upgradeInsecureRequests.value_or(USE_SSL) ?
                "; upgrade-insecure-requests" : ""
        )
    );

    // upgrade-insecure-requests not 100% necessary if using cloudflare with
    // auto http -> https upgrades
    // leave here anyway
}
