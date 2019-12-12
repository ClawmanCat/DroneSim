#pragma once

#include "Core.h"
#include "IEntity.h"

#include <iostream>

namespace DroneSim::Game {
    class EntityRocket : public IEntity<EntityRocket> {
    public:
        void update(void) {
            std::cout << "UpdateRocket\n";
        }
    };
}