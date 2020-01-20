#pragma once

#include "Core.h"
#include "IEntity.h"
#include "EntityTank.h"


namespace DroneSim::Game {
    // I'm still confused as to what these beams are actually supposed to represent...
    // If they were lasers coming from the sky, shouldn't they do damage around the point of impact,
    // rather than north of it?
    // And if it's some kind of flame, why does it cut off at the top?
    class EntityBeam : public IEntity<EntityBeam> {
    public:
        using IEntity<EntityBeam>::IEntity;
        using Base = IEntity<EntityBeam>;

        constexpr static bool             MayChange(void)        { return true;                 }
        constexpr static Vec2f            GetSize(void)          { return { 147, 318 };         }
        constexpr static Vec2f            GetBBSize(void)        { return { 100, 50  };         }
        constexpr static u32              GetFrameCount(void)    { return 3;                    }
        constexpr static std::string_view GetTexture(void)       { return "particle_beam";      }


        template <Team team> bool collides(const EntityTank<team>& tank) const {
            const Vec2f min = position - (Vec2f{ 0.5, 0.0 } * GetBBSize());
            const Vec2f max = position + (Vec2f{ 0.5, 1.0 } * GetBBSize());

            const Vec2f where  = tank.getPosition();
            const float radius = tank.GetRadius();

            const Vec2f delta = where - glm::clamp(where, min, max);
            return glm::dot(delta, delta) <= radius * radius;
        }


        void update(void);
    };
}