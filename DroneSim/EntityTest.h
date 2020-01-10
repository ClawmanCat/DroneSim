#pragma once

#include "Core.h"
#include "IEntity.h"
#include "ConstexprArg.h"
#include "TextureManager.h"

namespace DroneSim::Game {
    class EntityTest : public IEntity<EntityTest, Traits::ConstexprArg<bool, true>> {
    public:
        constexpr static auto GetObjectLayout(void) {
            return std::array {
                DRONESIM_GEN_LAYOUT_OBJ(EntityTest, position),
                DRONESIM_GEN_LAYOUT_OBJ(EntityTest, rotation),
                DRONESIM_GEN_LAYOUT_OBJ(EntityTest, frame)
            };
        }

        constexpr static Vec2f GetSize(void) {
            return { 0.3f, 0.3f };
        }

        constexpr static u32 GetFrameCount(void) {
            return 2;
        }

        static GLuint GetTextureID(void) {
            static GLuint id = GPU::TextureManager::instance().getTextureID("test");
            return id;
        }


        EntityTest(const Vec2f& position, float rotation) : position(position), rotation(rotation), frame(0) {}


        void update(void) {
            rotation += 0.001;
            frame = ((u32) rotation) & 1;
        }

        Vec2f getPosition(void) const {
            return position;
        }

        float getRotation(void) const {
            return rotation;
        }

        u32 getTextureFrame(void) const {
            return frame;
        }
    private:
        Vec2f position;
        float rotation;
        u32 frame;
    };
}