#pragma once

#include "Core.h"
#include "EntityRotating.h"
#include "Teams.h"


namespace DroneSim::Game {
    template <Team team> class EntityRocket : public EntityRotating<EntityRocket<team>> {
    public:
        // Being at this point marks a rocket for deletion.
        constexpr static Vec2f DESTRUCT_POINT = Vec2f{ std::numeric_limits<float>::infinity() };


        using Base = EntityRotating<EntityRocket<team>>;
        using EntityRotating<EntityRocket<team>>::EntityRotating;
        

        EntityRocket(const Vec2f& position, const Vec2f& target) : Base(position, glm::normalize(target - position)) {}


        constexpr static bool             MayChange(void)        { return true;                                               }
        constexpr static Vec2f            GetSize(void)          { return { 25.0, 24.0 };                                     }
        constexpr static u32              GetFrameCount(void)    { return 12;                                                 }
        constexpr static u32              GetFramesPerAnim(void) { return 3;                                                  }
        constexpr static std::string_view GetTexture(void)       { return (team == Team::RED) ? "rocket_red" : "rocket_blue"; }
        constexpr static float            GetRadius(void)        { return 10.0;                                               }

        void post_update(void);

        bool alive(void) const;


        // Target is a direction, not a position.
        Vec2f getDirection(void) const { return target; }
    };
}