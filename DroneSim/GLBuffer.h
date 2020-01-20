#pragma once

#include "Core.h"

#include <GL/glew.h>


namespace DroneSim::GPU {
    template <typename T> class GLBuffer {
    public:
        using value_type = T;


        constexpr static float OVERALLOCATE_FACTOR = 1.5f;


        GLBuffer(GLenum type = GL_POINTS) : vao(0), vbo(0), elems(0), alloc(0), drawType(type) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
        }


        GLBuffer(const std::vector<T>& data, GLenum type = GL_POINTS) : GLBuffer(type) {
            modify(0, data);
        }


        GLBuffer(const GLBuffer&) = delete;
        GLBuffer& operator=(const GLBuffer&) = delete;


        GLBuffer(GLBuffer&& other) : vao(other.vao), vbo(other.vbo), elems(other.elems), alloc(other.alloc), drawType(other.drawType) {
            other.vao = other.vbo = nullptr;
        }


        GLBuffer& operator=(GLBuffer&& other) {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);

            vao = other.vao;
            vbo = other.vbo;
            elems = other.elems;
            alloc = other.alloc;
            drawType = other.drawType;

            other.vao = other.vbo = nullptr;

            return *this;
        }


        ~GLBuffer(void) {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);
        }


        // Modifies the buffer by overwriting it with the provided data, starting at `where`.
        void modify(std::size_t where, const std::vector<T>& data) {
            if (data.size() == 0) return;
            if (where + data.size() > elems) resize(where + data.size());

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferSubData(GL_ARRAY_BUFFER, where * sizeof(T), data.size() * sizeof(T), &data[0]);
        }


        // Resizes the buffer to have `count` elements. Reallocates if neccesary.
        // If the new size is greater than the old one, 
        // the newly allocated part of the buffer must be filled using GLBuffer::modify before drawing the buffer.
        void resize(std::size_t count) {
            reserve(count * OVERALLOCATE_FACTOR);
            elems = count;
        }


        // Allocates GPU memory for the buffer. 
        // Giving a smaller value than the current allocation does not decrease the amount of allocated memory.
        void reserve(std::size_t count) {
            if (alloc >= count) return;

            GLuint newvbo = 0;

            glBindVertexArray(vao);

            glGenBuffers(1, &newvbo);
            glBindBuffer(GL_ARRAY_BUFFER, newvbo);

            glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
            glCopyBufferSubData(vbo, newvbo, 0, 0, alloc * sizeof(T));

            glDeleteBuffers(1, &vbo);

            vbo   = newvbo;
            alloc = count;
        }


        // Load the vertex attribute layout for elements of the given buffer.
        static void LoadLayout(GLuint program) {
            for (const auto& component : T::GetObjectLayout()) {
                // TODO: Remember indices between calls.
                GLint index = glGetAttribLocation(program, component.name);

                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, component.count, component.type, GL_FALSE, sizeof(T), (void*) component.offset);
            }

            glBindTexture(GL_TEXTURE_2D, T::GetTextureID());
        }


        GLuint      VAO (void) const { return vao;       }
        GLuint      VBO (void) const { return vbo;       }
        std::size_t size(void) const { return elems;     }
        GLenum      type(void) const { return drawType;  }
    private:
        GLuint vao, vbo;
        std::size_t elems, alloc;

        GLenum drawType;
    };
}