#pragma once

#include "StaticPolymorphism.h"
#include "IEntity.h"
#include "EntityTank.h"
#include "EntityRocket.h"
#include "EntityBackground.h"
#include "EntitySmoke.h"
#include "EntityExplosion.h"

namespace DroneSim::Game {
    // Entities earlier in the list are rendered first, and will appear behind entities further along.
    using Entities = Traits::DerivedClassList<IEntity, 
        EntityBackground,
        EntityTank<Team::RED>, 
        EntityTank<Team::BLUE>,
        EntityRocket<Team::RED>,
        EntityRocket<Team::BLUE>,
        EntityExplosion,
        EntitySmoke
    >;
}