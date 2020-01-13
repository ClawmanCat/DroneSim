#pragma once

#include "Core.h"
#include "PNGLoader.h"
#include "StringUtils.h"

#include <GL/glew.h>

#include <vector>
#include <string_view>

namespace DroneSim::GPU {
    class TextureManager {
    public:
        using TextureID = GLuint;

        
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;


        static TextureManager& instance(void) {
            static TextureManager instance { };
            return instance;
        }

        
        TextureID getTextureID(std::string_view name) {
            std::string path = StringUtils::cat(Paths::PATH_TEXTURES, name, ".png");

            // Check if texture is already loaded.
            auto it = ids.find(path);
            if (it != ids.end()) return it->second;

            // Load texture.
            auto data = FileIO::LoadPNG(path);
            auto id   = load_gl_texture(data);

            ids.insert({ path, id });

            return id;
        }
    private:
        HashMap<std::string, TextureID> ids;

        TextureManager(void) = default;


        static TextureID load_gl_texture(const FileIO::ImageData& image) {
            TextureID id;

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image.pixels[0]));
            glGenerateMipmap(GL_TEXTURE_2D);

            return id;
        }
    };
}