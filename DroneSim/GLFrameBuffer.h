#pragma once

#include "Core.h"
#include "TextureManager.h"
#include "GameConstants.h"

#include <GL/glew.h>

namespace DroneSim::GPU {
    namespace Detail {
        struct default_fbo_tag {};
    }


    class GLFrameBuffer {
    public:
        static GLFrameBuffer& Canvas(void) {
            static GLFrameBuffer canvas = GLFrameBuffer(0, Vec2ui{ Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT }, Detail::default_fbo_tag{});
            return canvas;
        }


        GLFrameBuffer(GLuint texture, const Vec2ui& size) : size(size) {
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

            GLenum target = GL_COLOR_ATTACHMENT0;
            glDrawBuffers(1, &target);
        }


        GLFrameBuffer(const GLFrameBuffer&) = delete;
        GLFrameBuffer& operator=(const GLFrameBuffer&) = delete;


        GLFrameBuffer(GLFrameBuffer&& other) : size(other.size), fbo(other.fbo) {
            other.fbo = 0;
        }


        GLFrameBuffer& operator=(GLFrameBuffer&& other) {
            if (fbo) glDeleteFramebuffers(1, &fbo);

            size = other.size;
            fbo  = other.fbo;

            other.fbo = 0;

            return *this;
        }


        ~GLFrameBuffer(void) {
            if (fbo) glDeleteFramebuffers(1, &fbo);
        }


        void bind(void) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glViewport(0, 0, size.x, size.y);
        }
    private:
        // For creating the default framebuffer object.
        GLFrameBuffer(GLuint fbo, const Vec2ui size, Detail::default_fbo_tag) : size(size), fbo(fbo) {}


        Vec2ui size;
        GLuint fbo;
    };
}