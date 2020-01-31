#include "EntityRocket.h"
#include "GameController.h"
#include "EntitySelectors.h"


namespace DroneSim::Game {
    // We edit the position and push explosions, so don't run this asynchronously.
    template <Team team> void EntityRocket<team>::post_update(void) {
        auto& controller = GameController::instance();

        position += ROCKET_MAX_SPEED * target;

        // Explode if near enemy.
        constexpr float tankradius = EntityTank<EnemyOf<team>()>::GetRadius();
        // // Not actually the radii squared. See SpatialPartition::nearby.
        constexpr float fakeDsq = (GetRadius() * GetRadius()) + (tankradius * tankradius);

        auto nearby = controller.getPartitions<EnemyOf<team>()>().nearby(position, fakeDsq, Utility::constexpr_sqrt(fakeDsq));

        for (auto* tank : nearby) {
            // Prevent concurrent access to tank health.
            static std::mutex mtx { };
            std::lock_guard lock(mtx);

            if (tank->alive()) {
                tank->hit(ROCKET_DAMAGE);

                std::lock_guard lock(controller.getEntitiesMtx());
                controller.getEntities().push_back(EntityExplosion(position));

                // Mark for deletion.
                target = DESTRUCT_POINT;
            }
        }
    }


    template <Team team> bool EntityRocket<team>::alive(void) const {
        return target != DESTRUCT_POINT;
    }


    // Specializations to avoid linker errors.
    template class EntityRocket<Team::RED>;
    template class EntityRocket<Team::BLUE>;
}