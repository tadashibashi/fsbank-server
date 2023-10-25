#pragma once
#include <aws/s3/S3Errors.h>

namespace Insound {

    class AwsS3Error : public std::runtime_error
    {
    public:
        explicit AwsS3Error(const Aws::S3::S3Error &error) :
            std::runtime_error(""), error(error), message(
                f("{}: {}", error.GetExceptionName(),
                    error.GetMessage()))
        { }

        const char *what() const noexcept
        {
            return message.c_str();
        }

    private:
        const Aws::S3::S3Error &error;
        const std::string message;
    };
}
