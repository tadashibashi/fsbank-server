#include "json.h"
#include <sstream>

std::string Insound::Json::dump() const {
    std::stringstream stream;
    this->stringify(stream);

    return stream.str();
}
