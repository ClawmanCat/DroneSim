#pragma once

#include "Core.h"

#include <SDL.h>
#include <GL/glew.h>

#include <string_view>

namespace DroneSim::Renderer {
    class WindowController {
    public:
        WindowController(std::string_view title, u32 w, u32 h, u32 x = SDL_WINDOWPOS_CENTERED, u32 y = SDL_WINDOWPOS_CENTERED) {
            SDL_Init(SDL_INIT_EVERYTHING);
            
            SDL_CreateWindow(title.data(), x, y, w, h, SDL_WINDOW_OPENGL);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
        }

        ~WindowController(void) {
            SDL_DestroyWindow(handle);
        }

        void onFrameStart(void) {

        }

        void onFrameEnd(void) {

        }
    private:
        SDL_Window* handle;
    };
}