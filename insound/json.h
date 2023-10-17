#include <Poco/JSON/Object.h>
#include <crow/returnable.h>

namespace Insound {
    class Json : public Poco::JSON::Object, public crow::returnable {
        Json(): Poco::JSON::Object(), crow::returnable("application/json") { }
        ~Json() = default;

    public:
        std::string dump() const override;
    };
}
