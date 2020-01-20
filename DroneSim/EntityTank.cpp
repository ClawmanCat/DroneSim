#include "EntityTank.h"
#include "GameController.h"
#include "EntitySelectors.h"
#include "EntityRocket.h"

namespace DroneSim::Game {
    template <Team team> EntityTank<team>::EntityTank(const Vec2f& pos, const Vec2f& target) : Base(pos, target) {
        data = new EntityTankAdditionalData{ ZVec2f, TANK_MAX_HEALTH, 0 };
    }


    template <Team team> EntityTank<team>::EntityTank(const EntityTank<team>& e) : Base(e) {
        data = new EntityTankAdditionalData{ e.data->force, e.data->health, e.data->reload_time };
    }


    template <Team team> EntityTank<team>::EntityTank(EntityTank<team>&& e) : Base(std::forward<EntityTank<team>>(e)) {
        data = e.data;
        e.data = nullptr;
    }


    template <Team team> EntityTank<team>::~EntityTank(void) {
        if (data) delete data;
    }


    template <Team team> EntityTank<team>& EntityTank<team>::operator=(const EntityTank<team>& e) {
        if (data) delete data;
        data = new EntityTankAdditionalData{ e.data->force, e.data->health, e.data->reload_time };

        static_cast<Base&>(*this).operator=(static_cast<const Base&>(e));
        return *this;
    }


    template <Team team> EntityTank<team>& EntityTank<team>::operator=(EntityTank<team>&& e) {
        if (data) delete data;
        data = e.data;
        e.data = nullptr;

        static_cast<Base&>(*this).operator=(static_cast<Base&&>(std::move(e)));
        return *this;
    }


    template <Team team> void EntityTank<team>::update(void) {
        if (data->health == 0) return;

        GameController& controller = GameController::instance();
        auto tanks = controller.getEntities().select<TankSelector>();

        // Collision detection.
        // TODO: Use proximity-friendly storage. (Quadtree or such.)
        tanks.forEach([&](const auto& tank) {
            // Don't collide with self.
            if (tank == *this) return;

            Vec2f direction  = getDirection();
            float distanceSq = glm::dot(direction, direction);
            
            // x2, since it is the sum of the radii of both tanks.
            float radiusSq = (2 * GetRadius() * GetRadius());
            radiusSq *= radiusSq;


            if (distanceSq < radiusSq) {
                push(glm::normalize(direction));
            }
        });


        // Move tank
        // This does not actually prevent anything from travelling faster than TANK_MAX_SPEED, 
        // but it is what was used in the original code.
        const Vec2f direction = glm::normalize(getDirection());
        position += (direction + data->force) * TANK_MAX_SPEED * 0.5f;
        
        data->force = ZVec2f;

        if (data->reload_time > 0) --data->reload_time;


        // Launch rocket
        if (data->reload_time == 0) {
            const auto& target = controller.findClosestEnemy(*this);
            controller.getEntities().push_back(EntityRocket<team>(position, target.getPosition()));

            data->reload_time = 200;
        }


        static_cast<Base&>(*this).update();
    }


    template <Team team> void EntityTank<team>::push(const Vec2f& direction, float magnitude) {
        data->force += (direction * magnitude);
    }


    template <Team team> void EntityTank<team>::hit(u16 value) {
        GameController& controller = GameController::instance();
        
        if (data->health <= value) {
            if (data->health != 0) {
                controller.getEntities().getT<EntitySmoke>().push_back(EntitySmoke(position + Vec2f{ 15, 15 }));
                data->health = 0;
            }
        } else data->health -= value;
    }


    template <Team team> void EntityTank<team>::setTarget(const Vec2f& where) {
        target = where;
    }


    template <Team team> bool EntityTank<team>::ready(void) const { 
        return data->reload_time == 0; 
    }


    template <Team team> bool EntityTank<team>::alive(void) const {
        return data->health > 0;
    }


    // Specializations to avoid linker errors.
    template class EntityTank<Team::RED>;
    template class EntityTank<Team::BLUE>;
}