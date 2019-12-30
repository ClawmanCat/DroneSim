#pragma once

#include "Core.h"
#include "IEntity.h"
#include "IRenderable.h"

#include <iostream>

namespace DroneSim::Game {
    class EntityRocket : public IEntity<EntityRocket>, public Render::IRenderable<EntityRocket> {
    public:
        constexpr static float WIDTH    = 25.0f;
        constexpr static float HEIGHT   = 24.0f;
        constexpr static u8 FRAME_COUNT = 12;


        EntityRocket(Vec2f position, float rotation = 0);
        EntityRocket(EntityRocket&& other);

        EntityRocket(const EntityRocket&) = delete;
        EntityRocket& operator=(const EntityRocket&) = delete;
        EntityRocket& operator=(EntityRocket&&) = delete;

        ~EntityRocket(void);


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