#include "json.h"
#include <sstream>

// ===== Lifetime =============================================================

Insound::Json::Json()
    : Poco::JSON::Object(), crow::returnable("application/json") {}

Insound::Json::Json(const Insound::Json &j)
    : Poco::JSON::Object(), crow::returnable("application/json")
{
    for (const auto &[k, v] : j)
        this->set(k, v);
}

Insound::Json::Json(const Poco::JSON::Object &obj)
    : Poco::JSON::Object(obj), crow::returnable("application/json")
{

}

Insound::Json &Insound::Json::operator=(const Insound::Json &j)
{
    // reset this object
    this->clear();

    // assign other json fields to this one
    for (const auto &[k, v] : j)
        this->set(k, v);

    return *this;
}

// ===== Member functions =====================================================

std::string Insound::Json::dump() const {
    std::stringstream stream;
    this->stringify(stream);

    return stream.str();
}
