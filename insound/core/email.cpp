#include "email.h"

#include <crow/utility.h>

#include <insound/core/json.h>
#include <insound/core/request.h>
#include <insound/core/settings.h>

#include <utility>

using Insound::Email::Attachment;
using Insound::Email::SendEmailOpts;

IN_JSON_META(Attachment, filename, content, path, href, httpHeaders,
            contentType, contentDisposition, cid, encoding, headers, raw);
IN_JSON_META(SendEmailOpts, from, to, cc, bcc, subject, text, html);

namespace Insound::Email
{
    SendEmail::SendEmail() : opts()
    {
        opts.from = Settings::emailAutomatedSender();
    }

    bool SendEmail::send() const
    {
        auto payload = JSON::stringify(this->opts);

        auto request = MakeRequest(Settings::emailEndpointURL(), "POST")
            .header("Authorization", Settings::emailAccessKey())
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
