#include "IEntity.h"
#include "GameController.h"


namespace DroneSim::Game::Detail {
    u32 GetSimulationFrameCount(void) {
        return GameController::instance().getFrameCount();
    }
}