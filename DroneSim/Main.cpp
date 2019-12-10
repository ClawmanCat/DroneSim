#include "CLCompiler.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <vector>
#include <string>
#include <iostream>

using namespace DroneSim::CL;

// Entry Point
int main(int argc, char** argv) {
    SDL_SetMainReady();
    std::vector<std::string> args { argv, argv + argc };

    std::vector<float> floats;
    for (unsigned i = 0; i < 1024; ++i) floats.push_back(i);

    auto program = CLCompiler::instance().compile<float, float*, float*, float*>("test.cl", "main");
    float c = 2;
    auto ba = CLBuffer<float>(floats, 1024, CL_MEM_READ_ONLY);
    auto bb = CLBuffer<float>(floats, 1024, CL_MEM_READ_ONLY);
    auto bc = CLBuffer<float>(floats, 1024, CL_MEM_WRITE_ONLY);

    program.run<false>(1024, 1024, c, ba, bb, bc);
    auto result = bc.fetch();

    unsigned i = 0;
    for (const auto& elem : result) std::cout << i++ << ": " << elem << '\n';

    return 0;
}