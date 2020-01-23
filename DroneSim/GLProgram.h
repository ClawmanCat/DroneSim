#pragma once

#include "Core.h"
#include "GLBuffer.h"
#include "StaticPolymorphism.h"
#include "VariadicSplitter.h"
#include "GLSetUniform.h"
#include "RenderableList.h"

#include <GL/glew.h>


namespace DroneSim::GPU {
    class GLProgram {
    public:
        GLProgram(GLuint program) : program(program) {}


        GLProgram(const GLProgram&) = delete;
        GLProgram& operator=(const GLProgram&) = delete;


        GLProgram(GLProgram&& other) : program(other.program), buffers(std::move(other.buffers)) {
            other.program = 0;
        }


        GLProgram& operator=(GLProgram&& other) {
            if (program) glDeleteProgram(program);

            program = other.program;
            buffers = std::move(other.buffers);

            return *this;
        }


        ~GLProgram(void) {
            if (program) glDeleteProgram(program);
        }


        void execute(void) {
            glUseProgram(program);

            buffers.forEach([&](const auto* buffer) {
                glBindVertexArray(buffer->VAO());
                glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO());

                no_cptr<decltype(buffer)>::LoadLayout(program);

                glDrawArrays(buffer->type(), 0, buffer->size());
            });
        }


        // Stores a reference to the given buffer. It will be drawn the next time execute is called.
        // The address of the buffer should remain valid until removeBuffer is called, but the contents of the buffer may change.
        template <typename T> void addBuffer(const GLBuffer<T>* buffer) {
            buffers.push_back<Wrapper<T>>(buffer);
        }


        template <typename T> void removeBuffer(const GLBuffer<T>* buffer) {
            buffers.erase<Wrapper<T>>(std::find_if(
                buffers.begin<Wrapper<T>>(),
                buffers.end<Wrapper<T>>(),
                [&](const auto* b) { return b->VAO() == buffer->VAO(); }
            ));
        }


        template <typename T> void setUniform(const char* name, const T& value) {
            glUseProgram(program);
            GLSetUniform::Set(glGetUniformLocation(program, name), value);
        }
    private:
        GLuint program;

        template <typename... Ts> using Wrapper = const GLBuffer<Ts...>*;
        Game::Renderables::Wrap<Wrapper>::PolyVector<> buffers;
    };
}