#pragma once
#include <insound/core/App.h>

#include <insound/server/middleware/CsrfCheck.h>
#include <insound/server/middleware/Security.h>

namespace Insound {
    using ServerType = Insound::App<Security, CsrfCheck>;

    class Server : public ServerType
    {
    public:
      Server();

    private:
      bool init() override;
    };
}
