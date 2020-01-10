#pragma once

#include "Core.h"
#include "GameConstants.h"

#include <SDL.h>

#include <string_view>

namespace DroneSim::Render {
    class WindowController {
    public:
        static WindowController& instance(void);

        WindowController(std::string_view title = Game::WINDOW_TITLE, u32 w = Game::WINDOW_WIDTH, u32 h = Game::WINDOW_HEIGHT, u32 x = SDL_WINDOWPOS_CENTERED, u32 y = SDL_WINDOWPOS_CENTERED);
        ~WindowController(void);

        void onFrameStart(void);
        void onFrameEnd(void);

        bool shouldClose(void) const;
    private:
        SDL_Window* handle;
    };
}