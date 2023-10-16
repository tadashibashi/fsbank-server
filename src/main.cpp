#include <iostream>
#include <fsbank.h>
#include <fsbank_errors.h>

#include <thread>

#include "server.h"
#include <cstdlib>


#define FSB_CHECK(result) do { \
        const FSBANK_RESULT res = (result); \
        if ( (res) != FSBANK_OK ) { \
            std::cerr << FSBank_ErrorString(res); \
            return (res); \
        } \
    } while(0)

const int DefaultPort = 1234;

int main (int argc, char *argv[])
{
    const auto NumCores = std::thread::hardware_concurrency();
    FSB_CHECK( FSBank_Init(FSBANK_FSBVERSION_FSB5, FSBANK_INIT_NORMAL, NumCores, ".fscache") );
    std::cout << "FSBank Initialized.\n";

    // Do stuff here

    FSB_CHECK ( FSBank_Release() );
    std::cout << "FSBank Released.\n";

    int PORT;
    if (auto portEnv = std::getenv("PORT"))
    {
        try {
            PORT = std::atoi(portEnv);
        } catch (...) {
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
