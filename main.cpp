#include <insound/App.h>
#include <iostream>

int main()
{
    std::cout <<
    "\n===== Insound Server ===============================================\n";

    Insound::App::instance().run();
    return 0;
}
