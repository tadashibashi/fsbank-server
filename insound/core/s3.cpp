#include "s3.h"
#include <insound/core/env.h>
#include <insound/core/errors/AwsS3Error.h>
#include <insound/core/util.h>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ChecksumAlgorithm.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/PutObjectRequest.h>

#include <sstream>

namespace Insound::S3 {
    // Global SDK options. May move config/close with its options elsewhere if
    // we end up using more than one AWS SDK.
    static const auto options = Aws::SDKOptions{};

    static std::string S3_BUCKET;
    static std::string AWS_ENDPOINT_URL;

    /**
     * Helper to create and return a new s3 client object.
     */
    static Aws::S3::S3Client getClient()
    {
        Aws::Client::ClientConfiguration config {};
        config.endpointOverride = AWS_ENDPOINT_URL;

        Aws::S3::S3Client client{config};

        return client;
    }

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
            throw AwsS3Error(result.GetError());

        std::vector<std::string> res;
        res.reserve(objects.size());

        for (auto &obj : objects)
        {
            res.emplace_back(obj.GetKey());
        }

        return res;
    }

    bool uploadFile(const std::string_view &key,
        const std::string &file)
    {
        auto client = getClient();

        // Make the request
        auto request = Aws::S3::Model::PutObjectRequest{};
        request.SetBucket(S3_BUCKET);
        request.SetKey(key.data());

        std::shared_ptr<Aws::IOStream> inputData =
            Aws::MakeShared<Aws::StringStream>("");
        *inputData << file;

        request.SetContentLength(file.length());
        request.SetBody(inputData);

        // Send request
        auto res = client.PutObject(request);

        if (!res.IsSuccess())
        {
            IN_ERR("S3 Upload Error: {}: {}",res.GetError().GetExceptionName(),
                res.GetError().GetMessage());
            return false;
        }

        return true;
    }

    std::optional<std::string> downloadFile(const std::string_view &key)
    {
        auto client = getClient();

        // Make the request
        auto request = Aws::S3::Model::GetObjectRequest{};
        request.SetBucket(S3_BUCKET);
        request.SetKey(key.data());

        auto res = client.GetObject(request);

        if (!res.IsSuccess())
        {
            IN_ERR("S3 Download Error: {}: {}",
                res.GetError().GetExceptionName(),
                res.GetError().GetMessage());
            return {};
        }

        std::stringstream stream;

        stream << res.GetResult().GetBody().rdbuf();
        return stream.str();
    }

    bool deleteFile(const std::string_view &key)
    {
        auto client = getClient();

        auto request = Aws::S3::Model::DeleteObjectRequest();
        request.SetBucket(S3_BUCKET);
        request.SetKey(key.data());

        auto result = client.DeleteObject(request);
        if (!result.IsSuccess())
        {
            IN_ERR("S3 Delete Object Error: {}: {}",
                result.GetError().GetExceptionName(),
                result.GetError().GetMessage());
            return false;
        }

        return true;
    }
}
