#pragma once

#include "Core.h"
#include "IGPUCompiler.h"
#include "GLProgram.h"

#include <GL/glew.h>

#include <tuple>
#include <array>

namespace DroneSim::GPU {
    class GLCompiler : public IGPUCompiler<GLCompiler, GLProgram> {
    public:
        using ProgramType = GLProgram;


        // Shader types: [File extension, GLenum value, required]
        constexpr static std::array ShaderTypes = {
            std::tuple{ ".vert.glsl",   GL_VERTEX_SHADER,           true  },
            std::tuple{ ".frag.glsl",   GL_FRAGMENT_SHADER,         true  },
            std::tuple{ ".geo.glsl",    GL_GEOMETRY_SHADER,         false },
            std::tuple{ ".tctrl.glsl",  GL_TESS_CONTROL_SHADER,     false },
            std::tuple{ ".teval.glsl",  GL_TESS_EVALUATION_SHADER,  false }
        };


        static GLCompiler& instance(void);


        GLCompiler(const GLCompiler&) = delete;
        GLCompiler(GLCompiler&&) = delete;

        GLCompiler& operator=(const GLCompiler&) = delete;
        GLCompiler& operator=(GLCompiler&&) = delete;


        ProgramType compile(const std::string& name);
    private:
        GLCompiler(void) = default;


        static void print_gl_errors(bool shader, GLenum parameter, GLuint id);
    };
}