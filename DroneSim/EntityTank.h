#pragma once

#include "Core.h"
#include "EntityRotating.h"
#include "Teams.h"
#include "GameConstants.h"


namespace DroneSim::Game {
    template <Team team> class EntityTank : public EntityRotating<EntityTank<team>> {
    public:
        constexpr static bool is_tank_tag = true;


        using Base = EntityRotating<EntityTank<team>>;
        

        EntityTank(const Vec2f& pos, float rotation) : Base(pos, rotation) {
            data = new EntityTankAdditionalData{ ZVec2f, pos, 0 };
        }

        EntityTank(const EntityTank<team>& e) : Base(e) {
            data = new EntityTankAdditionalData{ e.data->force, e.data->target, e.data->reload_time };
        }

        EntityTank(EntityTank<team>&& e) : Base(std::forward<EntityTank<team>>(e)) {
            data = e.data;
            e.data = nullptr;
        }

        ~EntityTank(void) {
            if (data) delete data;
        }


        EntityTank& operator=(const EntityTank& e) {
            if (data) delete data;
            data = new EntityTankAdditionalData{ e.data->force, e.data->target, e.data->reload_time };

            return static_cast<Base&>(*this).operator=(static_cast<const Base&>(e));
        }

        EntityTank& operator=(EntityTank&& e) {
            if (data) delete data;
            data = e.data;
            e.data = nullptr;

            return static_cast<Base&>(*this).operator=(static_cast<Base&&>(std::move(e)));
        }


        constexpr static bool             MayChange(void)        { return true;                                           }
        constexpr static Vec2f            GetSize(void)          { return { 14.0, 18.0 };                                 }
        constexpr static u32              GetFrameCount(void)    { return 12;                                             }
        constexpr static u32              GetFramesPerAnim(void) { return 3;                                              }
        constexpr static std::string_view GetTexture(void)       { return (team == Team::RED) ? "tank_red" : "tank_blue"; }
        constexpr static float            GetRadius(void)        { return 12.0;                                           }


        void update(void) {
            // This does not actually prevent anything from travelling faster than MAX_SPEED, 
            //but it is what was used in the original code.
            position += (data->target + data->force) * TANK_MAX_SPEED * 0.5f;
            data->force = ZVec2f;

            if (data->reload_time > 0) --data->reload_time;
        }


        void push(Vec2f direction, float magnitude = 1.0f) {
            data->force += (direction * magnitude);
        }
    private:
        // Minimize unused data when uploading to GPU by storing additional info as pointer.
        // TODO: Test performance with all data in object. (There will be some overhead from heap allocation as well)
        struct EntityTankAdditionalData {
            Vec2f force;
            Vec2f target;
            u8 reload_time;
        } *data;
    };
}