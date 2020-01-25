#pragma once

#include "Core.h"
#include "ILayoutObject.h"
#include "ITextureProvider.h"
#include "TextureManager.h"

#include <GL/glew.h>

#include <string>

namespace DroneSim::Game {
    // We can't move IEntity methods to a cpp file, since it's a template class,
    // so instead create this wrapper method to prevent circular dependencies.
    namespace Detail {
        extern u32 GetSimulationFrameCount(void);
    }


    template <typename D> class IEntity : public GPU::ILayoutObject<D>, public GPU::ITextureProvider<D> {
    public:
        constexpr static bool             MayChange(void)           { return D::MayChange();        }
        constexpr static Vec2f            GetSize(void)             { return D::GetSize();          }
        constexpr static u32              GetFrameCount(void)       { return D::GetFrameCount();    }
        constexpr static std::string_view GetTexture(void)          { return D::GetTexture();       }
        constexpr static u32              GetFrameRepeatCount(void) { return 1;                     }


        constexpr static auto GetObjectLayout(void) {
            return std::array {
                DRONESIM_GEN_LAYOUT_OBJ(D, position),
                DRONESIM_GEN_LAYOUT_OBJ(D, frame)
            };
        }


        static GLuint GetTextureID(void) {
            static GLuint id = GPU::TextureManager::instance().getTextureID(GetTexture());
            return id;
        }


        IEntity(const Vec2f& position, u32 frame = 0) : position(position), frame(frame) {}


        void update(void) {
            if (Detail::GetSimulationFrameCount() % GetFrameRepeatCount() != 0) return;

            if (frame < (GetFrameCount() - 1)) ++frame;
            else frame = 0;
        }


        Vec2f getPosition(void)     const { return position; }
        u32   getTextureFrame(void) const { return frame;    }


        void setPosition(const Vec2f& pos) { position = pos; }
    protected:
        IEntity(void) = default;

        Vec2f position;
        u32 frame;
    };
}