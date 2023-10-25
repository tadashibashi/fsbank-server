#include <insound/core/entrypoint.h>

class Server : public Insound::App<> {

};

namespace Insound {
    template<>
    App<> *getApp() { return new Server();  }
}
