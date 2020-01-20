#pragma once

#include "Core.h"
#include "Teams.h"
#include "IsSpecialization.h"

#include "EntityTank.h"
#include "EntityRocket.h"


namespace DroneSim::Game {
    struct TankSelector {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return Traits::IsVariableSpecialization<Team>::IsSpecialization<EntityTank, typename C::value_type>::value;
        }
    };


    template <Team team> struct EnemyTankSelector {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            if constexpr (Traits::IsVariableSpecialization<Team>::IsSpecialization<EntityTank, typename C::value_type>::value) return (C::value_type::GetTeam() != team);
            else return false;
        }
    };


    struct RocketSelector {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return Traits::IsVariableSpecialization<Team>::IsSpecialization<EntityRocket, typename C::value_type>::value;
        }
    };


    template <Team team> struct EnemyRocketSelector {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            if constexpr (Traits::IsVariableSpecialization<Team>::IsSpecialization<EntityRocket, typename C::value_type>::value) return (C::value_type::GetTeam() != team);
            else return false;
        }
    };
}