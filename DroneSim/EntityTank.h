#pragma once

#include "Core.h"
#include "EntityRotating.h"
#include "Teams.h"


namespace DroneSim::Game {
    template <Team team> class EntityTank : public EntityRotating<EntityTank<team>> {
    public:
        using Base = EntityRotating<EntityTank<team>>;
        using EntityRotating<EntityTank<team>>::EntityRotating;


        constexpr static bool             MayChange(void)        { return true;                                           }
        constexpr static Vec2f            GetSize(void)          { return { 14.0, 18.0 };                                 }
        constexpr static u32              GetFrameCount(void)    { return 12;                                             }
        constexpr static u32              GetFramesPerAnim(void) { return 3;                                              }
        constexpr static std::string_view GetTexture(void)       { return (team == Team::RED) ? "tank_red" : "tank_blue"; }
        constexpr static float            GetRadius(void)        { return 12.0;                                           }


        void update(void) {
            static_cast<Base*>(this)->update();
        }
    };
}