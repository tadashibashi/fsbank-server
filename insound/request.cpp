#include "request.h"
#include "Poco/URI.h"
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPClientSession.h>
#include <sstream>

namespace Insound::HttpMethod {
    const std::string Get = Poco::Net::HTTPRequest::HTTP_GET;
    const std::string Post = Poco::Net::HTTPRequest::HTTP_POST;
    const std::string Put = Poco::Net::HTTPRequest::HTTP_PUT;
    const std::string Patch = Poco::Net::HTTPRequest::HTTP_PATCH;
    const std::string Delete = Poco::Net::HTTPRequest::HTTP_DELETE;
}

Poco::JSON::Object::Ptr Insound::request(const std::string &url, const std::string &method, const Insound::Json *payload)
{
    Poco::URI uri(url);
    auto path = uri.getPathAndQuery();
    if (path.empty()) path = "/";
    std::cout << "method: " << method << '\n';
    auto session = Poco::Net::HTTPSClientSession(uri.getHost(), uri.getPort());
    auto req = Poco::Net::HTTPRequest(method, path, Poco::Net::HTTPMessage::HTTP_1_1);

    auto res = Poco::Net::HTTPResponse();

    if (payload)
    {
        // Write JSON payload if user provided it
        // stringify payload
        std::stringstream stream;
        payload->stringify(stream);
        auto data = stream.str();

        // set request headers for json object
        req.setContentType("application/json");
        req.setContentLength(data.size());

        // send the request and pipe the payload body
        auto &reqStream = session.sendRequest(req);
        reqStream << data;
    }
    else
    {
        // send the request
        session.sendRequest(req);
    }

    auto &resStream = session.receiveResponse(res);
    auto status = res.getStatus();
    if (status != 200)
    {
        throw "HTTPError code: " + std::to_string(status) + ": " + res.getReasonForStatus(status);
    }

    Poco::JSON::Parser parser;
    auto var = parser.parse(resStream);

    return var.extract<Poco::JSON::Object::Ptr>();
}
