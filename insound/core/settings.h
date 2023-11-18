/**
 * @file settings.h
 *
 * Contains various runtime settings for the application to access.
 *
 * Please make sure to call `configureEnv` from env.h before calling any of the
 * functions in this file.
 */
#pragma once
#include <string_view>

namespace Insound::Settings
{
    [[nodiscard]]
    std::string_view emailEndpointURL();

    [[nodiscard]]
    std::string_view emailAutomatedSender();

    [[nodiscard]]
    std::string_view emailAccessKey();

    [[nodiscard]]
    bool isProd();

    [[nodiscard]]
    std::string_view hostAddress();

    [[nodiscard]]
    std::string_view jwtSecret();

    [[nodiscard]]
    std::string_view mongoDBName();

    [[nodiscard]]
    std::string_view mongoURL();


    [[nodiscard]]
    std::string_view s3Bucket();

    [[nodiscard]]
    std::string_view awsEndpointURL();
}
