#pragma once
#include <insound/core/App.h>

#include <insound/server/CsrfCheck.h>

namespace Insound {
    using ServerType = Insound::App<CsrfCheck>;

    class Server : public ServerType
    {
    public:
      Server();

    private:
      bool init() override;
    };
}
