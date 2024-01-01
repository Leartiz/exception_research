#include <iostream>

#include "argh.h"

int main(int argc, char* argv[])
{
    argh::parser cmdl(argc, argv);

    std::cout << "[OK]";

    if (cmdl[{ "-v", "--verbose" }])
        std::cout << "Verbose, I am.\n";

    std::cout << "[OK]";
}
