#pragma once

#include "Core.h"
#include "EntityRotating.h"
#include "Teams.h"


namespace DroneSim::Game {
    template <Team team> class EntityRocket : public EntityRotating<EntityRocket<team>> {
    public:
        using Base = EntityRotating<EntityRocket<team>>;
        using EntityRotating<EntityRocket<team>>::EntityRotating;


        constexpr static bool             MayChange(void)        { return true;                                               }
        constexpr static Vec2f            GetSize(void)          { return { 24.0, 25.0 };                                     }
        constexpr static u32              GetFrameCount(void)    { return 12;                                                 }
        constexpr static u32              GetFramesPerAnim(void) { return 3;                                                  }
        constexpr static std::string_view GetTexture(void)       { return (team == Team::RED) ? "rocket_red" : "rocket_blue"; }


        void update(void) {
            static_cast<Base*>(this)->update();
        }
    };
}