#include "Helmet.h"
#include <insound/core/util.h>

void Insound::Helmet::before_handle(crow::request &req,
    crow::response &res, context &ctx)
{
    res.add_header("Strict-Transport-Security",
        "max-age=15552000; includeSubDomains");
    res.add_header("Cross-Origin-Embedder-Policy", "require-corp");
    res.add_header("Cross-Origin-Opener-Policy", "same-origin");
    res.add_header("Cross-Origin-Resource-Policy", "same-origin");
    res.add_header("Cross-Agent-Cluster", "?1");
    res.add_header("Referrer-Policy", "no-referrer");
    res.add_header("X-Content-Type-Options", "no-sniff");
    res.add_header("X-DNS-Prefetch-Control", "off");
    res.add_header("X-Download-Options", "noopen");
    res.add_header("X-Frame-Options", "SAMEORIGIN");
    res.add_header("X-Permitted-Cross-Domain-Policies", "none");
    res.add_header("X-XSS-Protection", "0");

    // set nonce
    auto nonce = genBytes(16);
    auto nonceStr = crow::utility::base64encode(nonce.data(), nonce.size());

    ctx.nonce = nonceStr;
    res.add_header("Content-Security-Policy",
        f(
            "base-uri 'self';"
            "font-src 'self';"
            "form-action 'self';"
            "frame-ancestors 'self';"
            "img-src 'self';"
            "object-src 'none';"
            "script-src 'nonce-{0}';"
            "style-src 'nonce-{0}';"
            //"upgrade-insecure-requests"
            , nonceStr));
}

void Insound::Helmet::after_handle(crow::request &req, crow::response &res,
    context &ctx)
{

}
