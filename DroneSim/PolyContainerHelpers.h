#pragma once

#include "Core.h"
#include "Pack.h"

#include <type_traits>

namespace DroneSim::Traits {
    template <typename...> struct NoWrapper {};
    template <template <typename...> typename Wrapper, typename T> using WrappedType = std::conditional_t<std::is_same_v<Wrapper<T>, NoWrapper<T>>, T, Wrapper<T>>;



    template <typename> struct GetTupleElems {};

    template <
        template <typename...> typename CC,
        template <typename...> typename WW,
        typename... Xs
    > struct GetTupleElems<std::tuple<CC<WW<Xs>>...>> {
        using type = Pack<Xs...>;

        template <typename... Ys> using container = CC<Ys...>;
        template <typename... Ys> using wrapper   = WW<Ys...>;
    };

    template <
        template <typename...> typename CC,
        typename... Xs
    > struct GetTupleElems<std::tuple<CC<Xs>...>> {
        using type = Pack<Xs...>;

        template <typename... Ys> using container = CC<Ys...>;
        template <typename... Ys> using wrapper   = NoWrapper<Ys...>;
    };

    template <typename... Xs> struct GetTupleElems<std::tuple<Xs...>> {
        using type = Pack<Xs...>;
    };
}