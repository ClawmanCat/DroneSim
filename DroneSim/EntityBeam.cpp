#include "EntityBeam.h"
#include "GameController.h"
#include "EntitySelectors.h"
#include "GameConstants.h"


namespace DroneSim::Game {
    void EntityBeam::update(void) {
        GameController& controller = GameController::instance();
        auto tanks = controller.getEntities().select<TankSelector>();

        tanks.forEach([&](auto& tank) {
            if (collides(tank)) tank.hit(BEAM_DAMAGE);
        });

        static_cast<Base&>(*this).update();
    }
}