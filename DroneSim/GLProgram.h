#pragma once

#include "Core.h"
#include "IGPURunnable.h"
#include "Pack.h"
#include "GLBuffer.h"
#include "Utility.h"
#include "GLSetUniform.h"

#include <GL/glew.h>

namespace DroneSim::GPU {
    class GLProgram : public IGPURunnable<GLProgram, GLBuffer, const char*> {
    public:
        GLProgram(GLuint program) : program(program) {}


        ~GLProgram(void) {
            if (program) glDeleteProgram(program);
        }


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


        template <typename T> constexpr static bool IsUniformAllowed(void) {
            using AllowedTypes = Traits::Pack<
                bool,
                i8, Vec2b, Vec3b, Vec4b,
                u8, Vec2ub, Vec3ub, Vec4ub,
                i16, Vec2s, Vec3s, Vec4s,
                u16, Vec2us, Vec3us, Vec4us,
                i32, Vec2i, Vec3i, Vec4i,
                u32, Vec2ui, Vec3ui, Vec4ui,
                f32, Vec2f, Vec3f, Vec4f,
                f64, Vec2d, Vec3d, Vec4d,
                Mat2x2f, Mat2x3f, Mat2x4f,
                Mat3x2f, Mat3x3f, Mat3x4f,
                Mat4x2f, Mat4x3f, Mat4x4f,
                Mat2x2d, Mat2x3d, Mat2x4d,
                Mat3x2d, Mat3x3d, Mat3x4d,
                Mat4x2d, Mat4x3d, Mat4x4d
            >;

            return AllowedTypes::Contains<T>();
        }


        void execute(void) {
            glUseProgram(program);

            for (const auto& buffer : buffers) {
                // Bind buffer
                glBindVertexArray(buffer.vao);
                glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

                // Set attribute locations
                buffer.loadAttribs(program);

                // Draw
                glDrawArrays(buffer.type, 0, buffer.size);
            }
        }

        template <typename T> GLuint addBuffer(const BufferType<T>& buffer) {
            buffers.push_back({ buffer.getVAO(), buffer.getVBO(), buffer.getType(), buffer.getSize(), buffer.getAttribLoader() });
            return buffer.getVAO();
        }

        void removeBuffer(GLuint id) {
            Utility::swap_erase(buffers, std::find_if(buffers.begin(), buffers.end(), [&](const BufferData& bd) { return bd.vao == id; }));
        }

        template <typename T> void setUniform(const T& uniform, const char* id) {
            glUseProgram(program);
            GLSetUniform::Set(glGetUniformLocation(program, id), uniform);
        }
    
        GLuint program;

        struct BufferData {
            GLuint vao, vbo;
            GLenum type;
            std::size_t size;
            FPtr<void, GLuint> loadAttribs;
        };

        std::vector<BufferData> buffers;
    };
}