#include "WindowController.h"

#include <GL/glew.h>

namespace DroneSim::Render {
    WindowController& WindowController::instance(void) {
        static WindowController instance{ };
        return instance;
    }


    WindowController::WindowController(std::string_view title, u32 w, u32 h, u32 x, u32 y) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);


        handle = SDL_CreateWindow(title.data(), x, y, w, h, SDL_WINDOW_OPENGL);
        if (!handle) terminate("Failed to create window.");

        SDL_GLContext context = SDL_GL_CreateContext(handle);
        if (!context) terminate("Failed to create OpenGL context");


        GLenum status = glewInit();
        if (status != GLEW_OK) terminate("Failed to initialize Glew.", status);


        SDL_GL_SetSwapInterval(0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

        glClearColor(0.35f, 0.35f, 0.35f, 1.0f);

        glEnableClientState(GL_VERTEX_ARRAY);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }


    WindowController::~WindowController(void) {
        SDL_DestroyWindow(handle);
    }


    void WindowController::onFrameStart(void) {
        glClear(GL_COLOR_BUFFER_BIT);
    }


    void WindowController::onFrameEnd(void) {
        SDL_GL_SwapWindow(handle);
    }


    bool WindowController::shouldClose(void) const {
        SDL_Event e;

        // TODO: Create class for managing user input.
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return true;
        }

        return false;
    }
}