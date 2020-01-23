#pragma once

#include "Core.h"
#include "EntityList.h"
#include "HealthBar.h"


namespace DroneSim::Game {
    using Renderables = typename Entities::template Append<HealthBar>;
}