//#define DRONESIM_RUN_OLD_VERSION


#ifndef DRONESIM_RUN_OLD_VERSION
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
        std::vector<std::string> args { argv, argv + argc };

        // Force window initialization.
        DroneSim::Render::WindowController::instance();
        DroneSim::Game::GameController::instance().start(std::move(args));

        return 0;
    }
#else
    #define SDL_MAIN_HANDLED
    #include "template.h"

    int main(int argc, char** argv) {
        SDL_SetMainReady();
        return old_main(argc, argv);
    }
#endif