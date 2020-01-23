#pragma once

#include "Core.h"
#include "EntityRotating.h"
#include "Teams.h"


namespace DroneSim::Game {
    template <Team team> class EntityRocket : public EntityRotating<EntityRocket<team>> {
    public:
        // Being at this point marks a rocket for deletion.
        constexpr static Vec2f DESTRUCT_POINT = Vec2f{ std::numeric_limits<float>::infinity() };
        // A large number, but not so large it will cause major rounding issues.
        constexpr static float LARGE_NUMBER = 1000.0f;

        using Base = EntityRotating<EntityRocket<team>>;
        using EntityRotating<EntityRocket<team>>::EntityRotating;


        // Move the target backwards along the vector the rocket will travel along,
        // so it will keep going if there is no more tank there by the time it arrives.
        // TODO: Keep target normalized and move it instead.
        EntityRocket(const Vec2f& position, const Vec2f& target) : Base(position, (target - position) * LARGE_NUMBER) {}


        constexpr static bool             MayChange(void)        { return true;                                               }
        constexpr static Vec2f            GetSize(void)          { return { 25.0, 24.0 };                                     }
        constexpr static u32              GetFrameCount(void)    { return 12;                                                 }
        constexpr static u32              GetFramesPerAnim(void) { return 3;                                                  }
        constexpr static std::string_view GetTexture(void)       { return (team == Team::RED) ? "rocket_red" : "rocket_blue"; }
        constexpr static float            GetRadius(void)        { return 10.0;                                               }


        void update(void);

        bool alive(void) const;


        template <typename Tank> bool collides(const Tank& tank) const {
            const Vec2f delta      = tank.getPosition() - position;
            const float distanceSq = Utility::dotself(delta);
            const float radiusSq   = Utility::square(Tank::GetRadius() + GetRadius());

            return distanceSq <= radiusSq;
        }
    };
}