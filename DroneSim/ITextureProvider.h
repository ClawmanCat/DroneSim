#pragma once

#include "Core.h"

#include <GL/glew.h>

namespace DroneSim::GPU {
    template <typename D> class ITextureProvider {
    public:
        static GLuint GetTextureID(void) {
            return static_cast<D>(*this)->GetTextureID();
        }
    };
}