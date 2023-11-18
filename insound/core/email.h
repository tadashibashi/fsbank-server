/**
 * @file email.h
 *
 * Contains SendEmail class for building and sending an email request.
 * `Email::config` should be successfully called before using any email
 * functionality.
 */
#pragma once
#include <optional>
#include <map>
#include <string>
#include <vector>

namespace Insound::Email {
    /**
     * Optional string alias
     */
    using OptString = std::optional<std::string>;

    /**
     * Options for an email attachment. Mirrors nodemailer attachment
     * object.
     */
    struct Attachment {
        /**
         * File name to set to the attachment
         */
        std::string filename;

        /**
         * File data content. No need to set this if `path` was set.
         */
        OptString content;

        /**
         * Local path to the file if you want to stream it instead of including
         * it via `content` (better for larger attachments).
         */
        OptString path;

        /**
         * A URL to the file. Data URI's are allowed as well.
         */
        OptString href;

        /**
         * Optional http headers to pass on with the `href` request, e.g.
         * {"Authorization": "Bearer ..."}
         */
        std::map<std::string, std::string> httpHeaders;

        /**
         * Optional content type for the attachment, if not set will be derived
         * from the `filename` property.
         */
        OptString contentType;

        /**
         * Optional content disposition type for the attachment, defaults to
         * "attachment".
         */
        OptString contentDisposition;

        /**
         * Optional content id for using inline images in HTML message source.
         * Example
         * std::string messageJson = R"({
         *     "html": "Embedded image: <img src='cid:my-cid-here'/>",
         *     "attachments": [
         *         {
         *             "filename": "image.png",
         *             "path": "/path/to/file",
         *             "cid": "my-cid-here"
         *         }
         *     ]
         * })"
         */
        OptString cid;

        /**
         * If set and `content` is set, then encodes the content to a Buffer
         * using the specified encoding. Example values: "base64", "hex",
         * "binary", etc. Useful if you want to use binary attachments in a
         * JSON formatted email object.
         */
        OptString encoding;

        /**
         * Custom headers for the attachment node. Same usage as with message
         * headers.
         */
        std::map<std::string, std::string> headers;

        /**
         * Is an optional special value that overrides the entire contents of
         * current mime node including mime headers. Useful if you want to
         * prepare node contents yourself.
         */
        OptString raw;
    };


    /**
     * Email message options. Mirrors basic nodemailer options.
     */
    struct SendEmailOpts {

        /**
         * Email address of the sender (must exist as an alias of the domain).
         */
        OptString from;

        /**
         * Comma-separated list of recipients
         */
        OptString to;

        /**
         * Comma-separated list of Cc recipients
         */
        OptString cc;

        /**
         * Comma-separated list of Bcc recipients
         */
        OptString bcc;

        /**
         * Subject of the email
         */
        OptString subject;

        /**
         * Plain text version of the message
         */
        OptString text;

        /**
         * HTML version of the message
         */
        OptString html;

        /**
         * A list of attachments
         */
        std::vector<Attachment> attachments;
    };


    /**
     * Convenience class for creating email object to send.
     * Uses cURL under the hood.
     *
     * @example
     * ```cpp
     *  Email::SendEmail()
     *      .from("example.sender@mail.com")
     *      .to("example.recipient@mail.com")
     *      .subject("Example subject")
     *      .html("<h1>Hello world</h1>")
     *      .text("Hello world")
     *      .send();
     * ```
     */
    class SendEmail
    {
    public:
        SendEmail();

        /**
        * Set who this email is from. By default, `from` is set to the
        * EMAIL_AUTOMATED_SENDER env variable, so this call overrides that
        * value.
        *
        * @param  sender e.g. "Bob Joe <example@mail.com>"
        *
        * @return reference to this SendEmail object for chaining.
        */
        SendEmail &from(std::string_view sender)
        {
            opts.from = sender;
            return *this;
        }

        /**
         * Set the address of whom this email is to be sent to, or a comma
         * -separated list if multiple recipients are intended
         *
         * @param  recipient e.g. "example@mail.com" or
         *                   "addr1@mail.com,addr2@mail.com,addr3@mail.com"
         *
         * @return reference to this SendEmail object for chaining.
         */
        SendEmail &to(std::string_view recipient)
        {
            opts.to = recipient;
            return *this;
        }

        /**
         * Set a comma-separated list of whom this email is to be cc'd to
         *
         * @param  cc e.g. "addr1@mail.com,addr2@mail.com,addr3@mail.com"
         *
         * @return    reference to this SendEmail object for chaining.
         */
        SendEmail &cc(std::string_view cc)
        {
            opts.cc = cc;
            return *this;
        }


        /**
         * Set a comma-separated list of whom this email is to be bcc'd to
         *
         * @param  bcc e.g. "addr1@mail.com,addr2@mail.com,addr3@mail.com"
         *
         * @return     reference to this SendEmail object for chaining.
         */
        SendEmail &bcc(std::string_view bcc)
        {
            opts.bcc = bcc;
            return *this;
        }


        /**
         * Set the subject of the email
         *
         * @param  subject - subject string to set
         *
         * @return reference to this SendEmail object for chaining.
         */
        SendEmail &subject(std::string_view subject)
        {
            opts.subject = subject;
            return *this;
        }


        /**
         * Set the text version of the message
         *
         * @param  text - text string to set
         *
         * @return reference to this SendEmail object for chaining.
         */
        SendEmail &text(std::string_view text)
        {
            opts.text = text;
            return *this;
        }


        /**
         * Set the html version of the message
         *
         * @param  html - html string to set
         *
         * @return reference to this SendEmail object for chaining.
         */
        SendEmail &html(std::string_view html)
        {
            opts.html = html;
            return *this;
        }


        /**
         * Append an attachment to the email. Ownership is handed off to
         * the email object and becomes moved/invalidated once passed.
         *
         * @param  attachment - attachment to add
         * @return reference to the SendEmail object for chaining.
         */
        SendEmail &attachment(Attachment *attachment);

        /**
         * Send the email
         * @return whether send succeeded.
         */
        bool send() const;

    private:
        SendEmailOpts opts;
    };

}
