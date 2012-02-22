#include <iostream>
#include <stdexcept>

#include "Badger.hpp"

// Include cstdlib for EXIT_SUCCESS with MinGW
#include <cstdlib>

int main()
{
    try
    {
        badger::Badger b;
        b.run();

        return EXIT_SUCCESS;
    }

    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
