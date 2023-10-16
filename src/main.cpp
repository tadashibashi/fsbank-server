#include <iostream>
#include <fsbank.h>
#include <fsbank_errors.h>

#include <thread>

#include "server.h"
#include <cstdlib>

const int DefaultPort = 1234;

int main (int argc, char *argv[])
{
    // Get the PORT from environment
    int PORT;
    if (auto portEnv = std::getenv("PORT"))
    {
        try {
            PORT = std::atoi(portEnv);
        } catch (...) {
            std::cerr << "Invalid PORT: not a number. Reverting to default " <<
                DefaultPort << '\n';
            PORT = DefaultPort;
        }
    }
    else
    {
        PORT = DefaultPort;
    }

    Insound::getApp().port(PORT).multithreaded().run();

    return 0;
}
