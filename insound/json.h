#include <Poco/JSON/Object.h>
#include <crow/returnable.h>

namespace Insound {
    class Json : public Poco::JSON::Object, public crow::returnable {
    public:
        Json();
        Json(const Poco::JSON::Object &obj);
        Json(const Json &j);
        Json &operator=(const Json &j);

        ~Json() = default;

        std::string dump() const override;

    };
}
