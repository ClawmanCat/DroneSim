#pragma once

#include "Core.h"
#include "Pack.h"
#include "PolyContainer.h"
#include "PolyVector.h"


namespace DroneSim::Traits {
    // Container of classes. (No CRTP restriction)
    // Can be used to construct a PolyContainer.
    template <typename... Ts> class ClassList {
    public:
        using Packed = Pack<Ts...>;


        template <template <typename...> typename W> using Wrap = ClassList<W<Ts>...>;
        template <typename... Xs> using Append = ClassList<Ts..., Xs...>;


        template <
            template <typename...> typename Container,
            template <typename...> typename Wrapper = NoWrapper
        > using PolyContainer = PolyContainer<Container, Wrapper, Ts...>;


        template <
            template <typename...> typename Wrapper = NoWrapper
        > using PolyVector = PolyVector<Wrapper, Ts...>;


        ClassList(void) = delete;

        template <typename X> constexpr static bool Contains(void) {
            return Pack<Ts...>::template Contains<X>();
        }
    private:
        template <typename H, typename... T>
        constexpr static bool VerifyUniqueness(void) {
            if constexpr (sizeof...(T) == 0) return true;
            else if constexpr (Pack<T...>::template Contains<H>()) return false;
            else return VerifyUniqueness<T...>();
        }


        static_assert(VerifyUniqueness<Ts...>());
    };



    // Container of classes. All classes in Derived should be CRTP-derivatives of the Base template.
    // Can be used to construct a PolyContainer.
    template <template <typename> typename Base, typename... Derived> class DerivedClassList : public ClassList<Derived...> {
    public:
        DerivedClassList(void) = delete;
    private:
        template <typename H, typename... T>
        constexpr static bool VerifyCRTP(void) {
            if constexpr (sizeof...(T) == 0) return true;
            else if constexpr (!std::is_base_of_v<Base<H>, H>) return false;
            else return VerifyCRTP<T...>();
        }


        static_assert(VerifyCRTP<Derived...>());
    };
}