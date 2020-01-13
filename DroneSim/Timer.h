#pragma once

#include "Core.h"

namespace DroneSim::Utility {
    class Timer {
    public:
        Timer(void) : start(steady_clock::now()) {}

        milliseconds elapsed(void) const {
            return duration_cast<milliseconds>(steady_clock::now() - start);
        }
    private:
        steady_clock::time_point start;
    };
}