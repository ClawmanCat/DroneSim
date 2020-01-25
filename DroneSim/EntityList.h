#pragma once

#include "StaticPolymorphism.h"
#include "IEntity.h"
#include "EntityTank.h"
#include "EntityRocket.h"
#include "EntityBackground.h"
#include "EntityBGOverlay.h"
#include "EntitySmoke.h"
#include "EntityExplosion.h"
#include "EntityBeam.h"
#include "EntityChar.h"


namespace DroneSim::Game {
    // Entities earlier in the list are rendered first, and will appear behind entities further along.
    using Entities = Traits::DerivedClassList<IEntity, 
        EntityBackground,
        EntityBGOverlay,
        EntityTank<Team::RED>, 
        EntityTank<Team::BLUE>,
        EntityRocket<Team::RED>,
        EntityRocket<Team::BLUE>,
        EntitySmoke,
        EntityExplosion,
        EntityBeam,
        EntityChar
    >;
}