#pragma once

#include "Core.h"

namespace DroneSim::Game {
    template <typename D> class IEntity {
    public:
        void update(void) {
            static_cast<D*>(this)->update();
        }
    protected:
        IEntity(void) = default;
    };
}