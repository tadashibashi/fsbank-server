#pragma once
#include <insound/core/App.h>

namespace Insound {
    using ServerType = Insound::App<>;

    class Server : public ServerType
    {
    public:
        Server() : Insound::App<>(Insound::AppOpts{
            .defaultPort=3000,
            .logLevel=Insound::LogLevel::Info,
        }) { }

    private:
      bool init() override;
    };
}
