/**
 * @file Server.h
 *
 * Contains class `Server`, which derives from Insound::App. This is the main
 * server object.
 */
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
      virtual ~Server();

    private:
      bool init() override;
    };
}
