#pragma once

#include "Core.h"
#include "IEntity.h"
#include "GameConstants.h"
#include "PNGLoader.h"


// Overlay for the background texture to draw tank tracks onto.
namespace DroneSim::Game {
    using FileIO::RGBA;


    class EntityBGOverlay : public IEntity<EntityBGOverlay> {
    public:
        EntityBGOverlay(void) : IEntity({ WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0 }) {
            texcache = FileIO::LoadPNG(StringUtils::cat(Paths::PATH_TEXTURES, GetTexture(), ".png"));
        }

        constexpr static bool             MayChange(void)     { return false;                           }
        constexpr static Vec2f            GetSize(void)       { return { WINDOW_WIDTH, WINDOW_HEIGHT }; }
        constexpr static u32              GetFrameCount(void) { return 1;                               }
        constexpr static std::string_view GetTexture(void)    { return "overlay";                       }


        void update(void) {
            // Update texture.
            glBindTexture(GL_TEXTURE_2D, GetTextureID());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texcache.w, texcache.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(texcache.pixels[0]));
        }


        void updateOverlay(const Vec2ui& where, const RGBA& data) {
            if (
                where.x < 0 || where.x >= texcache.w ||
                where.y < 0 || where.y >= texcache.h
            ) return;


            const u32 index = (texcache.w * texcache.h) - ((where.y + 1) * texcache.w) + where.x;
            texcache.pixels[index] = Blend(texcache.pixels[index], data);
        }
    private:
        FileIO::ImageData texcache;


        constexpr static RGBA Blend(const RGBA& x, const RGBA& y) {
            constexpr u8 MASK[2] = { 0x00, 0xFF };

            RGBA result = RGBA((x - y).rgb, 0xFF);

            auto gt = glm::greaterThan(result, x);
            result.r &= MASK[gt.r];
            result.g &= MASK[gt.g];
            result.b &= MASK[gt.b];

            return result;
        }
    };
}