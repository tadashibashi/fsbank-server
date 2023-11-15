#include "Server.h"
#include <insound/core/BankBuilder.h>
#include <insound/server/routes/api/test.h>

#include <crow/app.h>
#include <crow/utility.h>

#include <insound/core/definitions.h>
#include <insound/core/email.h>
#include <insound/core/env.h>
#include <insound/core/mongo.h>
#include <insound/core/s3.h>
#include <insound/core/util.h>
#include <insound/server/routes/api/auth.h>

#include <insound/core/middleware/Helmet.h>

namespace Insound {

    // Return param string without '?' in front of it, if any
    static inline std::string_view getQueryString(std::string_view rawUrl)
    {
        auto qpos = rawUrl.find_first_of('?');
        return (qpos == std::string_view::npos) ? "" :
            rawUrl.substr(qpos + 1);
    }

    Server::Server()
        : ServerType(Insound::AppOpts{
              .defaultPort = 3000,
              .logLevel = Insound::LogLevel::Info,
          }) {}

    static void catchall(const crow::request &req, crow::response &res)
    {
        static std::string HOST_ADDRESS{requireEnv("HOST_ADDRESS")};

        auto params = getQueryString(req.raw_url);

        res.redirect( f("{}/?redirect={}{}", HOST_ADDRESS,
            crow::utility::base64encode_urlsafe(req.url, req.url.size()),
            (params.empty() ? "" : f("&{}", params))
        ));

        res.end();
    }

    static void mainRoute(const crow::request &req, crow::response &res)
    {
        // Load html template
        crow::mustache::set_base(TEMPLATE_DIR "/");
        auto page = crow::mustache::load("index.html");

        // Grab nonce from Helmet middleware
        auto &helmet = Server::getContext<Helmet>(req);
        crow::mustache::context ctx({{"nonce", helmet.nonce}});

        auto &cookies = Server::getContext<crow::CookieParser>(req);

        auto bytes = genBytes();
        cookies.set_cookie("csrftoken",
            crow::utility::base64encode(bytes.data(), bytes.size()))
            .same_site(crow::CookieParser::Cookie::SameSitePolicy::Strict);

        // Render html
        res.end( page.render_string(ctx) );
    }

    bool Server::init()
    {
        // Populate environment variables, if .env file is available.
        configureEnv();

        if (auto buildResult = BankBuilder::initLibrary();
            buildResult != BankBuilder::OK)
            IN_ERR("FSBank builder failed to init: {}", buildResult);

        // Connect to S3, check for error
        bool result;
        result = S3::config();
        if (result)
            IN_LOG("S3 client initialized, connected to: {}",
                requireEnv("AWS_ENDPOINT_URL"));
        else
            IN_ERR("S3 client failed to initialize.");


        // Connect to MongoDB, check for error
        result = Mongo::connect();
        if (result)
            IN_LOG("MongoDB client initialized, connected to: {}",
                requireEnv("MONGO_URL"));
        else
            IN_ERR("MongoDB client failed to connect.");


        // Set up email API configuration, check for error
        result = Email::config();
        if (result)
            IN_LOG("Email API configured.");
        else
            IN_ERR("Email failed to configure.");


        // Mount routers
        mount<Auth>();
        mount<TestRouter>();

        // Main route
        CROW_ROUTE(this->internal(), "/")(mainRoute);

        // Workaround for catchall route bug.
        CROW_CATCHALL_ROUTE(this->internal())(catchall);

        return true;
    }

} // namespace Insound
