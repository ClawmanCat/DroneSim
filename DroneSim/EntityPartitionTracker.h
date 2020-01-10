#pragma once

#include "Core.h"
#include "EntityList.h"

#include <array>

namespace DroneSim::Game {
    class EntityPartitionTracker {
    public:
        constexpr static u32 PARTITION_SIZE_EXP = 7;
        constexpr static u32 PARTITION_SIZE     = 1 << PARTITION_SIZE_EXP;
        constexpr static u32 WORLD_BORDER       = 3000 / PARTITION_SIZE;


        EntityPartitionTracker(Entities::PolyContainer<std::vector>& entities) : entities(entities) {}

        template <typename T, typename = std::enable_if_t<Entities::Contains<T>>> 
        void relocate(const T& entity) {

        }
    private:
        Entities::PolyContainer<std::vector>& entities;
        std::array<u32, WORLD_BORDER * WORLD_BORDER> partition_limits;


        constexpr static u32 index(u32 x, u32 y) {
            x -= PARTITION_SIZE * (WORLD_BORDER / 2);
            y -= PARTITION_SIZE * (WORLD_BORDER / 2);


        }
    };
}