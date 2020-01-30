#pragma once

#include "Core.h"
#include "Teams.h"
#include "IsSpecialization.h"

#include "EntityTank.h"
#include "EntityRocket.h"


namespace DroneSim::Game {
    template <typename T> struct BufferPointerWrapper {
    private:
        // Take the template of container C, and give it new template arguments.
        template <typename> struct GetC {};
        template <template <typename...> typename C, typename... Ts> struct GetC<C<Ts...>> {
            template <template <typename...> typename X> using Apply = C<X<Ts...>>;
        };

        // Get the type contained in the buffer
        template <typename X, typename...> using NoBuffer = typename std::remove_pointer_t<X>::value_type;
    public:
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return T::template select<typename GetC<C>::template Apply<NoBuffer>, N>();
        }
    };


    struct SelectAll {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return true;
        }
    };


    template <typename T> struct Invert {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return !T::template select<C, N>();
        }
    };


    template <typename... Ts> struct And {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return (Ts::template select<C, N>() && ...);
        }
    };


    template <typename... Ts> struct Or {
        template <typename C, std::size_t N> constexpr static bool select(void) {
            return (Ts::template select<C, N>() || ...);
        }
    };


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