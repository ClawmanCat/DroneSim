#pragma once

#include "Core.h"
#include "IEntity.h"

#include <iostream>

namespace DroneSim::Game {
    class EntityTank : public IEntity<EntityTank> {
    public:
        void update(void) {
            std::cout << "UpdateTank\n";
        }
    };
}