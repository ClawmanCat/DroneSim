#include "EntityRocket.h"
#include "GameController.h"
#include "EntitySelectors.h"


namespace DroneSim::Game {
    template <Team team> void EntityRocket<team>::update(void) {
        GameController& controller = GameController::instance();
        auto tanks = controller.getEntities().select<EnemyTankSelector<team>>();


        // Move
        position += ROCKET_MAX_SPEED * glm::normalize(getDirection());


        // Explode if near enemy.
        tanks.forEach([&](auto& tank) {
            if (tank.alive() && collides(tank)) {
                tank.hit(ROCKET_DAMAGE);
                controller.getEntities().push_back(EntityExplosion(position));

                // Mark for deletion.
                target = ZVec2f;
            }
        });


        static_cast<Base*>(this)->update();
    }


    template <Team team> bool EntityRocket<team>::alive(void) const {
        return target != ZVec2f;
    }


    // Specializations to avoid linker errors.
    template class EntityRocket<Team::RED>;
    template class EntityRocket<Team::BLUE>;
}