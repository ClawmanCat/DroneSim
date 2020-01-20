#pragma once

#include "Core.h"
#include "IEntity.h"

namespace DroneSim::Game {
    class EntitySmoke : public IEntity<EntitySmoke> {
    public:
        using IEntity<EntitySmoke>::IEntity;


        constexpr static bool             MayChange(void)           { return true;             }
        constexpr static Vec2f            GetSize(void)             { return { 32, 32 };       }
        constexpr static u32              GetFrameCount(void)       { return 4;                }
        constexpr static std::string_view GetTexture(void)          { return "particle_smoke"; }
        constexpr static u32              GetFrameRepeatCount(void) { return 15;               }
    };
}