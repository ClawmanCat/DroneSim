#include "Core.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "GameController.h"
#include "WindowController.h"

#include <vector>
#include <string>
#include <iostream>

// Entry Point
int main(int argc, char** argv) {
    SDL_SetMainReady();
    std::vector<std::string> args{ argv, argv + argc };

    // Force window initialization.
    DroneSim::Render::WindowController::instance();
    DroneSim::Game::GameController::instance().start(std::move(args));

    return 0;
}