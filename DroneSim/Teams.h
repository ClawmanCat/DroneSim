#pragma once

#include "Core.h"

namespace DroneSim::Game {
    enum class Team { RED, BLUE };

    template <Team team> constexpr inline Team EnemyOf(void) {
        return (team == Team::RED) ? Team::BLUE : Team::RED;
    }
}