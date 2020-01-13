#pragma once

#include "Core.h"
#include "IEntity.h"

namespace DroneSim::Game {
    class EntityExplosion : public IEntity<EntityExplosion> {
    public:
        using IEntity<EntityExplosion>::IEntity;


        constexpr static bool             MayChange(void)        { return true;                 }
        constexpr static Vec2f            GetSize(void)          { return { 32, 32 };           }
        constexpr static u32              GetFrameCount(void)    { return 9;                    }
        constexpr static std::string_view GetTexture(void)       { return "particle_explosion"; }


        bool isFinished(void) const {
            return frame == GetFrameCount() - 1;
        }
    };
}