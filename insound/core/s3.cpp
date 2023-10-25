#include "s3.h"
#include <insound/core/env.h>
#include <insound/core/errors/AwsS3Error.h>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsV2Request.h>

namespace Insound::S3 {
    static const auto options = Aws::SDKOptions{};
    static std::string S3_BUCKET;
    static std::string AWS_ENDPOINT_URL;;
    bool config()
    {
        try {
            Aws::InitAPI(options);
            S3_BUCKET = requireEnv("S3_BUCKET");
            AWS_ENDPOINT_URL = requireEnv("AWS_ENDPOINT_URL");

            return true;
        } catch(const std::exception &e) {
            IN_ERR("Failed to initialize AWS sdk: {}", e.what());
            return false;
        } catch(...) {
            IN_ERR("Failed to initialize AWS sdk");
            return false;
        }

    }

    void close()
    {
        Aws::ShutdownAPI(options);
    }

    static Aws::S3::S3Client getClient()
    {
        Aws::Client::ClientConfiguration config {};
        config.endpointOverride = AWS_ENDPOINT_URL;

        Aws::S3::S3Client client{config};

        return client;
    }

    std::vector<std::string> listObjects(const std::string_view &prefix)
    {
        auto client = getClient();
        auto request = Aws::S3::Model::ListObjectsV2Request();
        request.SetBucket(S3_BUCKET);
        request.SetPrefix(prefix.data());

        auto result = client.ListObjectsV2(request);
        auto objects = result.GetResult().GetContents();

        std::cout << result.GetError().GetMessage() << '\n';
        if (!result.IsSuccess())
        {
            throw AwsS3Error(result.GetError());
            return {};
        }

        std::vector<std::string> res;
        res.reserve(objects.size());

        for (auto &obj : objects)
        {
            res.emplace_back(obj.GetKey());
        }

        return res;
    }
}
