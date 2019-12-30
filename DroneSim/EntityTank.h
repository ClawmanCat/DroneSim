#pragma once

#include "Core.h"
#include "IEntity.h"
#include "IRenderable.h"

#include <iostream>

namespace DroneSim::Game {
    class EntityTank : public IEntity<EntityTank>, public Render::IRenderable<EntityTank> {
    public:
        constexpr static float WIDTH    = 25.0f;
        constexpr static float HEIGHT   = 24.0f;
        constexpr static u8 FRAME_COUNT = 12;


        EntityTank(Vec2f position, float rotation = 0);
        EntityTank(EntityTank&& other);

        EntityTank(const EntityTank&) = delete;
        EntityTank& operator=(const EntityTank&) = delete;
        EntityTank& operator=(EntityTank&&) = delete;

        ~EntityTank(void);


        void update(void);


        const Vec2f& GetPosition(void) const;
        float GetRotation(void) const;
        const Vec2f& GetSize(void) const;
        u8 GetFrameCount(void) const;
        u16 GetTextureID(u8 frame) const;
    private:
        Vec2f position;
        float rotation;

        u32 renderID;
    };
}