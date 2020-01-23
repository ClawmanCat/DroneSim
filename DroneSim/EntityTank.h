#pragma once

#include "Core.h"
#include "EntityRotating.h"
#include "Teams.h"
#include "GameConstants.h"


namespace DroneSim::Game {
    template <Team team> class EntityTank : public EntityRotating<EntityTank<team>> {
    public:
        using Base = EntityRotating<EntityTank<team>>;
        using This = EntityTank<team>;
        

        EntityTank(const Vec2f& pos, const Vec2f& target);
        EntityTank(const EntityTank<team>& e);
        EntityTank(EntityTank<team>&& e);

        ~EntityTank(void);

        EntityTank& operator=(const EntityTank& e);
        EntityTank& operator=(EntityTank&& e);


        constexpr static bool             MayChange(void)        { return true;                                           }
        constexpr static Vec2f            GetSize(void)          { return { 14.0, 18.0 };                                 }
        constexpr static u32              GetFrameCount(void)    { return 12;                                             }
        constexpr static u32              GetFramesPerAnim(void) { return 3;                                              }
        constexpr static std::string_view GetTexture(void)       { return (team == Team::RED) ? "tank_red" : "tank_blue"; }
        constexpr static float            GetRadius(void)        { return 12.0;                                           }
        constexpr static Team             GetTeam(void)          { return team;                                           }


        void avoidCollision(void);
        void update(void);

        void push(const Vec2f& direction, float magnitude = 1.0f);
        void hit(u16 value);
        void setTarget(const Vec2f& where);

        bool ready(void) const;
        bool alive(void) const;


        u16 getHealth(void) const { return data->health; }


        template <typename T> constexpr bool operator==      (const T&    t) const { return false; }
        template <          > constexpr bool operator==<This>(const This& t) const { return ((void*) &t) == ((void*) this); }
    private:
        // Minimize unused data when uploading to GPU by storing additional info as pointer.
        // TODO: Test performance with all data in object. (There will be some overhead from heap allocation as well)
        struct EntityTankAdditionalData {
            Vec2f force;
            u16 health;
            u8 reload_time;
        } *data;
    };
}