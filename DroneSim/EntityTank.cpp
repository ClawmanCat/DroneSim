#include "EntityTank.h"
#include "GameController.h"
#include "EntitySelectors.h"
#include "EntityRocket.h"
#include "TextureManager.h"

namespace DroneSim::Game {
    template <Team team> EntityTank<team>::EntityTank(const Vec2f& pos, const Vec2f& target) : Base(pos, target), health(TANK_MAX_HEALTH) {
        data = new EntityTankAdditionalData{ pos, ZVec2f, ZVec2f, 0 };
    }


    template <Team team> EntityTank<team>::EntityTank(const EntityTank<team>& e) : Base(e), health(e.health) {
        data = new EntityTankAdditionalData{ e.data->tmp_position, e.data->target, e.data->force, e.data->reload_time };
    }


    template <Team team> EntityTank<team>::EntityTank(EntityTank<team>&& e) : Base(std::forward<EntityTank<team>>(e)), health(e.health) {
        data = e.data;
        e.data = nullptr;
    }


    template <Team team> EntityTank<team>::~EntityTank(void) {
        if (data) delete data;
    }


    template <Team team> EntityTank<team>& EntityTank<team>::operator=(const EntityTank<team>& e) {
        if (data) delete data;
        data = new EntityTankAdditionalData{ e.data->tmp_position, e.data->target, e.data->force, e.data->reload_time };

        health = e.health;

        static_cast<Base&>(*this).operator=(static_cast<const Base&>(e));
        return *this;
    }


    template <Team team> EntityTank<team>& EntityTank<team>::operator=(EntityTank<team>&& e) {
        if (data) delete data;
        data = e.data;
        e.data = nullptr;

        health = e.health;

        static_cast<Base&>(*this).operator=(static_cast<Base&&>(std::move(e)));
        return *this;
    }


    template <Team team> void EntityTank<team>::avoidCollision(void) {
        // Don't push if this tank is dead.
        if (health == 0) return;

        GameController& controller = GameController::instance();


        Utility::multi_for_each(
            [&](const auto* tank) {
                // Prevent the tank from pushing itself.
                if constexpr (std::is_same_v<
                    no_cref<decltype(*this)>,
                    no_cref<decltype(*tank)>
                >) if (*this == *tank) return;


                const Vec2f delta = position - tank->getPosition();

                const float dsq = glm::dot(delta, delta);
                // Not actually the radii squared. See SpatialPartition::nearby.
                const float rsq = 2 * GetRadius() * GetRadius();

                // Scale force to distance to decrease tank vibrations.
                push(glm::normalize(delta), Utility::scale(rsq - dsq, { 0.0f, rsq }, { 0.0f, 4.0f }));
            },

            // Yes these numbers aren't correct. See SpatialPartition::nearby.
            controller.getPartitions<Team::BLUE>().nearby(position, 2 * GetRadius() * GetRadius(), Utility::constexpr_sqrt(2.0f) * GetRadius()),
            controller.getPartitions<Team::RED >().nearby(position, 2 * GetRadius() * GetRadius(), Utility::constexpr_sqrt(2.0f) * GetRadius())
        );
    }


    template <Team team> void EntityTank<team>::update(void) {
        if (health == 0) return;

        GameController& controller = GameController::instance();

        avoidCollision();

        // Move tank
        const Vec2f direction = glm::normalize(getDirection()) + data->force;
        
        data->tmp_position = position + direction * TANK_MAX_SPEED * 0.5f;
        data->force = ZVec2f;


        // Launch rocket
        if (data->reload_time > 0) --data->reload_time;

        if (data->reload_time == 0) {
            const auto* target = controller.getPartitions<EnemyOf<team>()>().closest(position, [](auto* tank) { return tank->alive(); });
            data->target = target->getPosition();
        }


        // Draw tracks
        const static Vec2f HALF_SIZE = 0.5f * GetSize();

        // Should be threadsafe, we never write to the same pixel multiple times.
        auto& overlay = controller.getEntities().getT<EntityBGOverlay>()[0];
        overlay.updateOverlay((Vec2ui) (position - HALF_SIZE), { 0x80, 0x80, 0x80, 0xFF });


        static_cast<Base&>(*this).update();
    }


    template <Team team> void EntityTank<team>::post_update(void) {
        if (health == 0) return;

        GameController& controller = GameController::instance();


        position = data->tmp_position;

        if (data->reload_time == 0) {
            std::lock_guard lock(controller.getEntitiesMtx());
            controller.getEntities().push_back(EntityRocket<team>(position, data->target));

            data->reload_time = 200;
        }
    }


    template <Team team> void EntityTank<team>::push(const Vec2f& direction, float magnitude) {
        data->force += (direction * magnitude);
    }


    template <Team team> void EntityTank<team>::hit(u16 value) {
        GameController& controller = GameController::instance();
        
        if (health <= value) {
            if (health != 0) {
                std::lock_guard(controller.getEntitiesMtx());
                controller.getEntities().getT<EntitySmoke>().push_back(EntitySmoke(position + Vec2f{ 15, 15 }));

                health = 0;
            }
        } else health -= value;
    }


    template <Team team> void EntityTank<team>::setTarget(const Vec2f& where) {
        target = where;
    }


    template <Team team> bool EntityTank<team>::ready(void) const { 
        return data->reload_time == 0; 
    }


    template <Team team> bool EntityTank<team>::alive(void) const {
        return health > 0;
    }


    // Specializations to avoid linker errors.
    template class EntityTank<Team::RED>;
    template class EntityTank<Team::BLUE>;
}