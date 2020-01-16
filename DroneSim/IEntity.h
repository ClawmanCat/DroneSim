#pragma once

#include "Core.h"
#include "ILayoutObject.h"
#include "ITextureProvider.h"
#include "TextureManager.h"

#include <GL/glew.h>

#include <string>

namespace DroneSim::Game {
    template <typename D> class IEntity : public GPU::ILayoutObject<D>, public GPU::ITextureProvider<D> {
    public:
        constexpr static bool is_tank_tag = false;


        constexpr static bool             MayChange(void)        { return D::MayChange();        }
        constexpr static Vec2f            GetSize(void)          { return D::GetSize();          }
        constexpr static u32              GetFrameCount(void)    { return D::GetFrameCount();    }
        constexpr static std::string_view GetTexture(void)       { return D::GetTexture();       }


        constexpr static auto GetObjectLayout(void) {
            return std::array {
                DRONESIM_GEN_LAYOUT_OBJ(D, position),
                DRONESIM_GEN_LAYOUT_OBJ(D, rotation),
                DRONESIM_GEN_LAYOUT_OBJ(D, frame)
            };
        }


        static GLuint GetTextureID(void) {
            static GLuint id = GPU::TextureManager::instance().getTextureID(GetTexture());
            return id;
        }


        IEntity(const Vec2f& position, float rotation) : position(position), rotation(rotation), frame(0) {}


        void update(void) {
            if (frame < (GetFrameCount() - 1)) ++frame;
            else frame = 0;
        }


        Vec2f getPosition(void)     const { return position; }
        float getRotation(void)     const { return rotation; }
        u32   getTextureFrame(void) const { return frame;    }


        void setPosition(const Vec2f& pos) { position = pos; }
        void setRotation(float rot)        { rotation = rot; }
    protected:
        IEntity(void) = default;

        Vec2f position;
        float rotation;
        u32 frame;
    };
}