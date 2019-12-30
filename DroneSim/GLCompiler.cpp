#include "GLCompiler.h"
#include "FileIO.h"
#include "StringUtils.h"

#include <iostream>


namespace DroneSim::GPU {
    GLCompiler& GLCompiler::instance(void) {
        static GLCompiler instance{ };
        return instance;
    }


    GLCompiler::ProgramType GLCompiler::compile(const std::string& name) {
        std::vector<GLuint> shaders;

        // Compile shaders.
        for (const auto& [ext, type, required] : ShaderTypes) {
            auto source = FileIO::ReadText(StringUtils::cat(Paths::PATH_SHADERS, name, ext));

            if (source) {
                std::string   sourcestr = StringUtils::cat(source.value(), " \n ");
                const GLchar* sourceptr = sourcestr.c_str();
                const GLint   lengthval = sourcestr.length();

                GLuint id = glCreateShader(type);


                glShaderSource(id, 1, &sourceptr, &lengthval);
                glCompileShader(id);
                print_gl_errors(true, GL_COMPILE_STATUS, id);


                shaders.push_back(id);
            } else if (required) {
                terminate("Shader pipeline is missing required stage.");
            }
        }

        // Link program.
        GLuint program = glCreateProgram();
        for (GLuint shader : shaders) glAttachShader(program, shader);

        glLinkProgram(program);
        print_gl_errors(false, GL_LINK_STATUS, program);

        for (GLuint shader : shaders) {
            glDeleteShader(shader);
            glDetachShader(program, shader);
        }


        return GLProgram(program);
    }


    void GLCompiler::print_gl_errors(bool shader, GLenum parameter, GLuint id) {
        GLint status = 0;
        shader ? glGetShaderiv(id, parameter, &status) : glGetProgramiv(id, parameter, &status);

        if (status == GL_FALSE) {
            #ifndef NDEBUG
                GLint length = 0;
                shader ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

                std::string message;
                message.resize(length);

                shader ? glGetShaderInfoLog(id, length, &length, &message[0]) : glGetProgramInfoLog(id, length, &length, &message[0]);

                std::cout << "An OpenGL error has occured:\n";
                std::cout << message;
            #endif

            terminate("An OpenGL error occurred.");
        } else {
            #ifndef NDEBUG
                std::cout << "No OpenGL errors detected.\n";
            #endif
        }
    }
}