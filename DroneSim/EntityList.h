#pragma once

#include "StaticPolymorphism.h"
#include "IEntity.h"
#include "EntityTank.h"

namespace DroneSim::Game {
    using Entities = Traits::DerivingClassList<IEntity, EntityTank<Team::RED>, EntityTank<Team::BLUE>>;
}