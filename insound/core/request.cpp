#include "request.h"

#include <insound/core/errors/CurlError.h>
#include <insound/core/errors/CurlHeaderError.h>
#include <curl/curl.h>

#include <cassert>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace Insound::HttpMethod {
    const std::string Get = "GET";
    const std::string Post = "POST";
    const std::string Put = "PUT";
    const std::string Patch = "PATCH";
    const std::string Delete = "DELETE";
}

static auto InitOk = curl_global_init(CURL_GLOBAL_ALL);

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    auto bytes = size * nmemb;
    std::string &file = *static_cast<std::string *>(userp);
    file = std::string{(char *)buffer, (char *)buffer + bytes};

    return bytes;
}

class CurlHeaders {
public:
    CurlHeaders() : list() { }
    ~CurlHeaders()
    {
        if (list)
            curl_slist_free_all(list);
    }

    void append(std::string_view name, std::string_view value)
    {
        auto header = f("{}: {}", name, value);
        auto temp = curl_slist_append(list, header.c_str());
        if (!temp)
            throw std::runtime_error("Error occurred while appending header "
                "to curl_slist");
        list = temp;
    }

    auto getList() { return list; }
private:
    curl_slist *list;
};


struct CurlContext {
    CurlContext() : headers(), curl(curl_easy_init())
    {
        assert(InitOk == CURLE_OK);
    }

    ~CurlContext()
    {
        curl_easy_cleanup(curl);
    }

    void clear()
    {
        curl_easy_reset(curl);
        headers = CurlHeaders();
    }

    CurlHeaders headers;
    CURL *curl;
};

struct Insound::MakeRequest::Impl {
    Impl() : ctx(new CurlContext) { }
    explicit Impl(std::shared_ptr<CurlContext> &ctx) : ctx(ctx) { }

    std::shared_ptr<CurlContext> ctx;

    CURL *curl() { return ctx->curl; }
    CurlHeaders &headers() { return ctx->headers; }
};



Insound::MakeRequest::MakeRequest() : m(new Impl)
{ }


Insound::MakeRequest::MakeRequest(std::string_view url,
    std::string_view method) : m(new Impl)
{
    this->url(url).method(method);
}


Insound::MakeRequest::MakeRequest(const MakeRequest &other) : m(new Impl(other.m->ctx))
{

}


Insound::MakeRequest &Insound::MakeRequest::operator=(const MakeRequest &other)
{
    m->ctx = other.m->ctx;
    return *this;
}


Insound::MakeRequest::~MakeRequest()
{

}


Insound::MakeRequest &
Insound::MakeRequest::url(std::string_view url)
{
    auto result = curl_easy_setopt(m->curl(), CURLOPT_URL, url.data());
    if (result != CURLE_OK)
        throw CurlError(result);

    return *this;
}


Insound::MakeRequest &
Insound::MakeRequest::method(std::string_view method)
{
    auto code = curl_easy_setopt(m->curl(), CURLOPT_CUSTOMREQUEST,
        method.data());
    if (code != CURLE_OK)
        throw CurlError(code);
    return *this;
}

Insound::MakeRequest &
Insound::MakeRequest::body(std::string_view body)
{
    auto code = curl_easy_setopt(m->curl(), CURLOPT_POSTFIELDS, body.data());
    if (code != CURLE_OK)
        throw CurlError(code);
    return *this;
}

Insound::MakeRequest &
Insound::MakeRequest::header(std::string_view name,
    std::string_view value)
{
    m->headers().append(name, value);
    return *this;
}

std::string
Insound::MakeRequest::send()
{
    CURLcode result;

    if (m->headers().getList())
    {
        result = curl_easy_setopt(m->curl(), CURLOPT_HTTPHEADER,
            m->headers().getList());
        if (result != CURLE_OK)
            throw CurlError(result);
    }

    result = curl_easy_setopt(m->curl(), CURLOPT_WRITEFUNCTION, write_data);
    if (result != CURLE_OK)
        throw CurlError(result);

    std::string resBody;
    result = curl_easy_setopt(m->curl(), CURLOPT_WRITEDATA, &resBody);
    if (result != CURLE_OK)
        throw CurlError(result);

    result = curl_easy_perform(m->curl());
    if (result != CURLE_OK)
        throw CurlError(result);

    return resBody;
}


std::string
Insound::MakeRequest::getHeader(std::string_view name) const
{
    curl_header *header;
    auto result = curl_easy_header(m->curl(), name.data(), 0, CURLH_HEADER, -1,
        &header);
    if (result != CURLHE_OK)
        throw CurlHeaderError(result);

    return header->value ? header->value : "";
}


std::vector<std::string>
Insound::MakeRequest::getHeaders(std::string_view name) const
{
    // Get the first header
    curl_header *header;
    auto result = curl_easy_header(m->curl(), name.data(), 0, CURLH_HEADER, -1,
        &header);

    if (result != CURLHE_OK)
        throw CurlHeaderError(result);

    // Find total headers with same name and collect the rest, if any
    size_t amount = header->amount;
    if (!amount) return {}; // should not occur, but just in case

    std::vector<std::string> res;
    res.reserve(amount);

    res.emplace_back(header->value);

    for (size_t i = 1; i < amount; ++i)
    {
        result = curl_easy_header(m->curl(), name.data(), i, CURLH_HEADER, -1,
            &header);
        if (result != CURLHE_OK)
            throw CurlHeaderError(result);

        if (!header->value) continue; // should not occur, but just in case

        res.emplace_back(header->value);
    }

    return res;
}


long
Insound::MakeRequest::getCode() const
{
    long code;
    auto result = curl_easy_getinfo(m->curl(), CURLINFO_HTTP_CODE, &code);
    if (result != CURLE_OK)
        throw CurlError(result);

    return code;
}


std::string Insound::request(std::string_view url, std::string_view method,
    std::string_view payload)
{
    assert(InitOk == CURLE_OK);

    auto curl = curl_easy_init();

    if (!curl) throw "Error initializaing curl";

    try {
        CurlHeaders headers;
        headers.append("Content-Type", "application/json");
        std::string resBody;
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resBody);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.getList());
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip, deflate");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.data());

        auto result = curl_easy_perform(curl);
        if (result != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            throw CurlError(result);
        }

        long code;
        result = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        if (result != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            throw CurlError(result);
        }


        curl_easy_cleanup(curl);
        return resBody;
    }
    catch (...)
    {
        curl_easy_cleanup(curl);
        throw;
    }
}
