#include "email.h"

#include <crow/utility.h>

#include <insound/core/env.h>
#include <insound/core/request.h>

#include <utility>

namespace Insound::Email
{
    static std::string EMAIL_ENDPOINT_URL;
    static std::string EMAIL_AUTOMATED_SENDER;
    static std::string EMAIL_ACCESS_KEY;

    bool config()
    {
        try {
            // Set up constants for the email api will use
            EMAIL_ENDPOINT_URL = requireEnv("EMAIL_ENDPOINT_URL");
            EMAIL_AUTOMATED_SENDER = requireEnv("EMAIL_AUTOMATED_SENDER");
            EMAIL_ACCESS_KEY = requireEnv("EMAIL_ACCESS_KEY");
            EMAIL_ACCESS_KEY = "Basic " + crow::utility::base64encode(
                EMAIL_ACCESS_KEY + ":", EMAIL_ACCESS_KEY.size() + 1);
            return true;
        }
        catch (const std::exception &e)
        {
            IN_ERR("Exception while configuring email: {}", e.what());
            return false;
        }
        catch(...)
        {
            IN_ERR("Unknown error while configuring email.");
            return false;
        }
    }

    SendEmail::SendEmail() : opts()
    {
        opts.from = EMAIL_AUTOMATED_SENDER;
    }


    bool SendEmail::send() const
    {
        std::string payload;
        glz::write_json(this->opts, payload);

        auto request = MakeRequest(EMAIL_ENDPOINT_URL, "POST")
            .header("Authorization", EMAIL_ACCESS_KEY)
            .header("Content-Type", "application/json")
            .body(payload);

        auto resBody = request.send();

        auto code = request.getCode();
        if (code < 200 || code > 299)
        {
            IN_WARN("Request failed: {}: {}", code, resBody);
            return false;
        }

        IN_LOG(resBody);

        return true;
    }


    SendEmail &SendEmail::attachment(Attachment *attachment)
    {
        opts.attachments.emplace_back(std::move(*attachment));
        return *this;
    }


} // namespace Insound::Email
