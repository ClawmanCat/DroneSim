#pragma once

#include "Core.h"

#include <SDL.h>

#include <string_view>

namespace DroneSim::Render {
    class WindowController {
    public:
        constexpr static u32 WINDOW_WIDTH  = 1280;
        constexpr static u32 WINDOW_HEIGHT = 720;
        constexpr static std::string_view WINDOW_TITLE = "Drone Simulator";


        static WindowController& instance(void);

        WindowController(std::string_view title, u32 w, u32 h, u32 x = SDL_WINDOWPOS_CENTERED, u32 y = SDL_WINDOWPOS_CENTERED);
        ~WindowController(void);

        void onFrameStart(void);
        void onFrameEnd(void);

        bool shouldClose(void) const;
    private:
        SDL_Window* handle;
    };
}