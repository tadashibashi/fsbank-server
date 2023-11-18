#include "settings.h"
#include <insound/core/env.h>

#include <crow/utility.h>

#include <string>

namespace Insound::Settings
{
    std::string_view emailEndpointURL()
    {
        static std::string s_url = requireEnv("EMAIL_ENDPOINT_URL").data();
        return s_url;
    }


    std::string_view emailAutomatedSender()
    {
        static std::string s_sender =
            requireEnv("EMAIL_AUTOMATED_SENDER").data();
        return s_sender;
    }


    static inline std::string getEmailAccessKey()
    {
        std::string accessKey = requireEnv("EMAIL_ACCESS_KEY").data();
        accessKey = "Basic " + crow::utility::base64encode(
            accessKey + ":", accessKey.size() + 1);
        return accessKey;
    }


    std::string_view emailAccessKey()
    {
        static std::string s_accessKey = getEmailAccessKey();
        return s_accessKey;
    }


    bool isProd()
    {
        static bool s_production = getEnv("PRODUCTION") == "true" ||
            getEnv("NODE_ENV") == "production";
        return s_production;
    }


    std::string_view hostAddress()
    {
        static std::string s_hostAddress = requireEnv("HOST_ADDRESS").data();
        return s_hostAddress;
    }


    std::string_view mongoDBName()
    {
        static std::string s_mongoDBName = requireEnv("MONGO_DBNAME").data();
        return s_mongoDBName;
    }


    std::string_view jwtSecret()
    {
        static std::string s_jwtSecret = requireEnv("JWT_SECRET").data();
        return s_jwtSecret;
    }


    std::string_view mongoURL()
    {
        static std::string s_mongoURL = requireEnv("MONGO_URL").data();
        return s_mongoURL;
    }

    std::string_view s3Bucket()
    {
        static std::string s_s3Bucket = requireEnv("S3_BUCKET").data();
        return s_s3Bucket;
    }

    std::string_view awsEndpointURL()
    {
        static std::string s_endpoint = requireEnv("AWS_ENDPOINT_URL").data();
        return s_endpoint;
    }
}
