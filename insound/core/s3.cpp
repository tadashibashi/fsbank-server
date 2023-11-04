#include "s3.h"
#include "insound/core/ZipWriter.h"
#include <insound/core/env.h>
#include <insound/core/errors/AwsS3Error.h>
#include <insound/core/util.h>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ChecksumAlgorithm.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/DeleteBucketRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/DeleteObjectsRequest.h>
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
    static std::optional<Aws::S3::S3Client> sClient{};

    /**
     * Helper to get s3 pseudo-singleton client object.
     */
    static Aws::S3::S3Client &getClient()
    {
        if (!sClient)
        {
            Aws::Client::ClientConfiguration config {};
            config.endpointOverride = AWS_ENDPOINT_URL;

            try {
                sClient = Aws::S3::S3Client{config};
            }
            catch(...)
            {
                IN_ERR("Failed to get S3 client");
                throw;
            }
        }

        return sClient.value();
    }

    /**
     * Create a bucket
     *
     * @param name - bucket name
     *
     * @return whether op was successful. If a bucket already exists, the op
     *         is considered a success. On false, an error will be logged.
     */
    bool createBucket(std::string_view bucket)
    {
        // Check if bucket exists, add if not
        auto &client = getClient();
        auto buckets = client.ListBuckets();
        if (!buckets.IsSuccess())
            throw AwsS3Error(buckets.GetError());
        bool bucketExists = false;
        for (auto &bucket : buckets.GetResult().GetBuckets())
        {
            if (bucket.GetName() == S3_BUCKET)
            {
                bucketExists = true;
                break;
            }
        }

        if (!bucketExists)
        {
            Aws::S3::Model::CreateBucketRequest request;
            request.SetBucket(S3_BUCKET.c_str());
            auto createResult = client.CreateBucket(request);
            if (!createResult.IsSuccess())
            {
                IN_ERR("S3 Download Error: {}: {}",
                    createResult.GetError().GetExceptionName(),
                    createResult.GetError().GetMessage());
            }

            return createResult.IsSuccess();
        }

        return true;
    }

    bool config()
    {
        try {
            Aws::InitAPI(options);
            S3_BUCKET = requireEnv("S3_BUCKET");
            AWS_ENDPOINT_URL = requireEnv("AWS_ENDPOINT_URL");

            createBucket(S3_BUCKET);

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

    std::vector<std::string> listObjects(std::string_view prefix)
    {
        auto &client = getClient();
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

    bool uploadFile(std::string_view key,
        const std::string &file)
    {
        auto &client = getClient();

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

    std::optional<std::string> downloadFile(std::string_view key)
    {
        auto &client = getClient();

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

    bool deleteFile(std::string_view key)
    {
        auto &client = getClient();

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

    bool deleteFiles(const std::vector<std::string> &keys)
    {
        if (keys.empty()) return false;

        auto &client = getClient();
        auto request = Aws::S3::Model::DeleteObjectsRequest();

        Aws::S3::Model::Delete del;
        for (auto &key : keys)
        {
            del.AddObjects(Aws::S3::Model::ObjectIdentifier().WithKey(key));
        }

        request.SetDelete(del);
        request.SetBucket(S3_BUCKET);

        auto result = client.DeleteObjects(request);

        if (!result.IsSuccess())
        {
            IN_ERR("S3 Delete Objects Error: {}: {}",
                result.GetError().GetExceptionName(),
                result.GetError().GetMessage());
            return false;
        }

        return result.GetResult().GetDeleted().size() == keys.size();
    }

    bool deleteFolder(std::string_view folderKey)
    {
        if (folderKey.empty()) return false; // no folder to delete

        // Ensure the key ends with a slash, indicating a folder (not other
        // keys with the same prefix, but outside of that folder)
        std::string key = folderKey.data();
        if (!key.ends_with('/'))
            key += '/';

        return deleteFiles( listObjects(key) );
    }

    bool dropBucket__permanent__(std::string_view bucket)
    {
        auto &client = getClient();
        // Delete all files in bucket
        {
            auto result = Aws::S3::Model::ListObjectsV2Outcome{};
            auto request = Aws::S3::Model::ListObjectsV2Request{};
            request.SetBucket(S3_BUCKET);
            std::vector<std::string> list;
            do {
                result = client.ListObjectsV2(request);

                if (!result.IsSuccess())
                {
                    IN_ERR("Failed to list objects in dropBucket: {}",
                        result.GetError().GetMessage());
                    break;
                }
                if (result.GetResult().GetContents().empty())
                    break;

                for (auto &obj : result.GetResult().GetContents())
                    list.emplace_back(obj.GetKey());

                if (!deleteFiles(list))
                {
                    IN_ERR("Failed to delete files in dropBucket");
                    break;
                }

                list.clear();
            } while (result.GetResult().GetIsTruncated());
        }

        // Delete bucket
        auto request = Aws::S3::Model::DeleteBucketRequest();
        request.SetBucket(bucket.data());

        auto result = client.DeleteBucket(request);
        if (!result.IsSuccess())
        {
            IN_ERR("S3 Delete Bucket Error: {}: {}",
                result.GetError().GetExceptionName(),
                result.GetError().GetMessage());
        }

        return result.IsSuccess();
    }


    std::optional<std::string> zipFolder(std::string_view folderKey)
    {
        std::string key = folderKey.data();
        if (!folderKey.ends_with('/'))
            key += '/';

        auto list = listObjects(key);

        if (list.empty()) // no files to zip
            return {};
        try
        {
            ZipWriter writer;

            for (auto &key : list)
            {
                auto file = downloadFile(key);
                if (file)
                {
                    auto shortenedKey = key.substr(folderKey.length());
                    writer.addFile(shortenedKey, file.value());
                }
            }

            if (writer.numEntries() == 0)
                return {};

            auto copy = writer.copy();
            return std::string(copy.begin(), copy.end());
        }
        catch (const std::exception &e)
        {
            IN_ERR("Error while zipping file: {}", e.what());
            return {};
        }
        catch(...)
        {
            IN_ERR("Unknown error while zipping file.");
            return {};
        }

    }
}
