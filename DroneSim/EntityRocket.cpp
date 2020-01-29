#include "EntityRocket.h"
#include "GameController.h"
#include "EntitySelectors.h"


namespace DroneSim::Game {
    template <Team team> void EntityRocket<team>::update(void) {
        GameController& controller = GameController::instance();

        // Move
        position += ROCKET_MAX_SPEED * target;


        // Explode if near enemy.
        constexpr float tankradius = EntityTank<EnemyOf<team>()>::GetRadius();
        // // Not actually the radii squared. See SpatialPartition::nearby.
        constexpr float fakeDsq = (GetRadius() * GetRadius()) + (tankradius * tankradius);

        auto nearby = controller.getPartitions<EnemyOf<team>()>().nearby(position, fakeDsq, Utility::constexpr_sqrt(fakeDsq));

        for (auto* tank : nearby) {
            if (tank->alive()) {
                tank->hit(ROCKET_DAMAGE);
                controller.getEntities().push_back(EntityExplosion(position));

                // Mark for deletion.
                target = DESTRUCT_POINT;
            }
        }


        static_cast<Base*>(this)->update();
    }


    template <Team team> bool EntityRocket<team>::alive(void) const {
        return target != DESTRUCT_POINT;
    }


    // Specializations to avoid linker errors.
    template class EntityRocket<Team::RED>;
    template class EntityRocket<Team::BLUE>;
}