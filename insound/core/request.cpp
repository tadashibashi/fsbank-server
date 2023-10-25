#include "request.h"

#include <insound/core/errors/CurlError.h>
#include <curl/curl.h>

#include <cassert>
#include <sstream>

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

    void append(const std::string &name, const std::string &value)
    {
        auto header = f("{}: {}", name, value);
        list = curl_slist_append(list, header.c_str());
        if (!list)
            throw "Error occurred while appending header to curl_slist";
    }

    auto getList() { return list; }
private:
    curl_slist *list;
};


std::string Insound::request(const std::string &url, const std::string &method, const std::string &payload)
{
    assert(InitOk == CURLE_OK);

    auto curl = curl_easy_init();

    if (!curl) throw "Error initializaing curl";

    try {
        CurlHeaders headers;
        headers.append("Content-Type", "application/json");
        std::string resBody;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resBody);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.getList());
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip, deflate");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

        auto result = curl_easy_perform(curl);
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
