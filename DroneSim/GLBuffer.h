#pragma once

#include "Core.h"
#include "IGPUBuffer.h"
#include "ILayoutObject.h"
#include "ITextureProvider.h"

#include <GL/glew.h>

namespace DroneSim::GPU {
    namespace Detail {
        template <typename T> constexpr static bool AllowedType = (std::is_base_of_v<ILayoutObject<T>, T> && std::is_base_of_v<ITextureProvider<T>, T>);
    }


    template <typename T, typename = std::enable_if_t<Detail::AllowedType<T>>> class GLBuffer : public IGPUBuffer<GLBuffer<T>, T, true, false> {
    public:
        using value_type = T;

        constexpr static float BUFFER_INITIAL_ALLOC    = 1.2;
        constexpr static float BUFFER_OVERALLOC_FACTOR = 1.5;


        GLBuffer(GLenum type, std::size_t size) : type(type), elems(0), alloc(size) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, alloc * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
        }


        GLBuffer(GLenum type, const std::vector<T>& data) : GLBuffer(type, data.size() * BUFFER_INITIAL_ALLOC) {
            modify(0, data);
        }


        ~GLBuffer(void) {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);
        }


        GLBuffer(const GLBuffer&) = delete;
        GLBuffer& operator=(const GLBuffer&) = delete;


        GLBuffer(GLBuffer&& other) : vao(other.vao), vbo(other.vbo), type(other.type), elems(other.elems), alloc(other.alloc) {
            other.vao = other.vbo = 0;
        }


        GLBuffer& operator=(GLBuffer&& other) {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);

            vao   = other.vao;
            vbo   = other.vbo;
            type  = other.type;
            elems = other.elems;
            alloc = other.alloc;

            other.vao = other.vbo = 0;
        }


        void modify(std::size_t where, std::size_t count, const T* data) {
            if (where + count > elems) resize(std::max<u64>(where + count, elems));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, where * sizeof(T), count * sizeof(T), data);

            elems = std::max(elems, where + count);
        }


        void modify(std::size_t where, const std::vector<T>& data) {
            if (data.size() > 0) modify(where, data.size(), &data[0]);
        }


        void resize(std::size_t newcount) {
            // New size fits in buffer, don't reallocate.
            if (newcount < alloc) return;

            // New size is too big, reallocate.
            GLuint newvbo;

            glBindVertexArray(vao);

            glGenBuffers(1, &newvbo);
            glBindBuffer(GL_ARRAY_BUFFER, newvbo);

            glBufferData(GL_ARRAY_BUFFER, newcount * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
            glCopyBufferSubData(vbo, newvbo, 0, 0, elems);

            glDeleteBuffers(1, &vbo);

            vbo   = newvbo;
            alloc = newcount;
        }


        GLuint getVAO (void) const { return vao;  }
        GLuint getVBO (void) const { return vbo;  }
        GLenum getType(void) const { return type; }
        std::size_t getSize(void) const { return elems; }
        FPtr<void, GLuint> getAttribLoader(void) const { return &GLBuffer::BindAttributes; }
    private:
        GLuint vao, vbo;
        GLenum type;

        // number of elements stored and max capacity in elements.
        std::size_t elems, alloc;


        static void BindAttributes(GLuint program) {
            for (const auto& component : T::GetObjectLayout()) {
                // TODO: Remember indices between calls.
                GLint index = glGetAttribLocation(program, component.name);

                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, component.count, component.type, GL_FALSE, sizeof(T), (void*) component.offset);
            }

            glBindTexture(GL_TEXTURE_2D, T::GetTextureID());
        }
    };
}