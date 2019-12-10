#pragma once

#include <iostream>
#include <string_view>

namespace DroneSim {
    // Exceptions are disabled for performance reasons, use this for fatal errors.
    inline void terminate(std::string_view message, int code = -1) {
        std::cout << "A fatal error occurred:\n" << message << "\n";
        std::cout << "Press enter to exit...";

        std::cin.get();

        std::exit(code);
    }
}