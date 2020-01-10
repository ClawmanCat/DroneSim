#pragma once

#include "Core.h"
#include "IEntity.h"
#include "TextureManager.h"
#include "Teams.h"
#include "ConstexprArg.h"
#include "Utility.h"

#include <string_view>

namespace DroneSim::Game {
    template <Team team> class EntityTank : public IEntity<EntityTank<team>, Traits::ConstexprArg<bool, true>> {
    public:
        const static inline std::string TEXTURE = (team == Team::RED) ? "tank_red" : "tank_blue";

        constexpr static auto GetObjectLayout(void) {
            return std::array{
                DRONESIM_GEN_LAYOUT_OBJ(EntityTank, position),
                DRONESIM_GEN_LAYOUT_OBJ(EntityTank, rotation),
                DRONESIM_GEN_LAYOUT_OBJ(EntityTank, frame)
            };
        }

        constexpr static Vec2f GetSize(void) {
            return { 14.0, 18.0 };
        }

        constexpr static u32 GetFrameCount(void) {
            return 12;
        }

        static GLuint GetTextureID(void) {
            static GLuint id = GPU::TextureManager::instance().getTextureID(TEXTURE);
            return id;
        }


        EntityTank(const Vec2f& position, float rotation) : position(position), rotation(rotation), frame(0) {}


        void update(void) {
            rotation += 0.0001;
            if (rotation > TAU) rotation -= TAU;

            u32 frame_base = 0;
            if (rotation < 0.25 * PI || rotation > TAU - (0.25 * PI)) frame_base = 0; // Right
            else if (rotation < 0.75 * PI) frame_base = 6; // Down
            else if (rotation < 1.25 * PI) frame_base = 3; // Right
            else frame_base = 9; // Up


            // If already on this base advance to next frame, otherwise switch frames.
            if (frame - frame_base < 2) ++frame;
            else frame = frame_base;
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