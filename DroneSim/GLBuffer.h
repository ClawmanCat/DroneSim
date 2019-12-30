#pragma once

#include "Core.h"
#include "IGPUBuffer.h"
#include "ILayoutObject.h"

#include <GL/glew.h>

namespace DroneSim::GPU {
    namespace Detail {
        template <typename T> constexpr static bool AllowedType =
            std::is_fundamental_v<T> ||
            std::is_base_of_v<ILayoutObject<T>, T>;
    }


    template <typename T, typename = std::enable_if_t<Detail::AllowedType<T>>> class GLBuffer : public IGPUBuffer<GLBuffer<T>, T, true, false> {
    public:
        GLBuffer(GLenum type, std::size_t size) : size(size), type(type) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
        }


        GLBuffer(GLenum type, const std::vector<T>& data) : GLBuffer(type, data.size()) {
            modify(0, data);
        }


        ~GLBuffer(void) {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);
        }


        GLBuffer(const GLBuffer&) = delete;
        GLBuffer& operator=(const GLBuffer&) = delete;


        GLBuffer(GLBuffer&& other) : vao(other.vao), vbo(other.vbo), type(other.type), size(other.size) {
            other.vao = other.vbo = 0;
        }


        GLBuffer& operator=(GLBuffer&& other) {
            if (vao) glDeleteVertexArrays(1, &vao);
            if (vbo) glDeleteBuffers(1, &vbo);

            vao  = other.vao;
            vbo  = other.vbo;
            type = other.type;
            size = other.size;

            other.vao = other.vbo = 0;
        }




        void modify(std::size_t where, const std::vector<T>& data) {
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferSubData(GL_ARRAY_BUFFER, where * sizeof(T), data.size() * sizeof(T), &data[0]);
        }


        GLuint getVAO (void) const { return vao;  }
        GLuint getVBO (void) const { return vbo;  }
        GLenum getType(void) const { return type; }
        std::size_t getSize(void) const { return size; }
        FPtr<void, GLuint> getAttribLoader(void) const { return &GLBuffer::BindAttributes; }
    private:
        std::size_t size;
        GLenum type;
        GLuint vao, vbo;


        static void BindAttributes(GLuint program) {
            if constexpr (std::is_fundamental_v<T>) {
                // Fundamental type
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 1, LayoutComponent::GetComponentType<T>(), GL_FALSE, sizeof(T), (void*) 0);
            } else {
                // Type with layout information
                for (const auto& component : T::GetObjectLayout()) {
                    GLint index = glGetAttribLocation(program, component.name.c_str());

                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index, component.count, component.type, GL_FALSE, sizeof(T), (void*) component.offset);
                }
            }
        }
    };
}