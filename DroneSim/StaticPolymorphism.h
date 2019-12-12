#pragma once

#include "Core.h"
#include "FindTupleType.h"

#include <type_traits>
#include <tuple>

namespace DroneSim::Traits {
    template <typename...> struct NoWrapperType {};

    // Storage for a list of classes.
    template <template <typename T> typename CRTPBase, typename... Classes> class DerivingClassList {
    private:
        // Check if the provided classes actually have the given CRTP base.
        template <typename Head, typename... Tail> constexpr static bool VerifyCRTP(void) {
            if constexpr (sizeof...(Tail) == 0 && std::is_base_of_v<CRTPBase<Head>, Head>) return true;
            
            if constexpr (sizeof...(Tail) == 0) return false;
            else return VerifyCRTP<Tail...>();
        }

        static_assert(VerifyCRTP<Classes...>());


        // Does the given parameter pack contain T?
        template <typename T, typename Head, typename... Tail> constexpr static bool ContainsImpl(void) {
            if constexpr (std::is_same_v<T, Head>) return true;
            
            if constexpr (sizeof...(Tail) == 0) return false;
            else return ContainsImpl<T, Tail...>();
        }
    public:
        // If T is a class from Classes, the return type of this method is the type stored in a PolymorphicContainer for Classes.
        template <typename T, template <typename... Ts> typename WrapperType> constexpr static auto GetContainedType(void) {
            if constexpr (std::is_same_v<WrapperType<T>, NoWrapperType<T>>) return std::declval<T>();
            else return std::declval<WrapperType<T>>();
        }


        // Creates a container that fakes polymorphism for Classes.
        template <
            template <typename...> typename Container,
            template <typename...> typename Wrapper
        > class PolymorphicContainer : public std::tuple<Container<decltype(GetContainedType<Classes, Wrapper>())>...> {
        public:
            using self = std::tuple<Container<decltype(GetContainedType<Classes, Wrapper>())>...>;
            using typename self::tuple;

            template <typename... Ts> using container = Container<Ts...>;
            template <typename... Ts> using wrapper   = Wrapper<Ts...>;

            constexpr static std::size_t Size = sizeof...(Classes);
        };

        // Does the list of classes contain T?
        template <typename T> constexpr static bool Contains(void) {
            return ContainsImpl<T, Classes...>();
        }
    };

    // std::get but for PolymorphicContainer
    template <std::size_t N, typename Container> constexpr static auto PolyContainerGet(Container& c) {
        return std::get<N>(static_cast<typename Container::self>(c));
    }

    // std::tuple_size but for PolymorphicContainer
    template <typename Container> constexpr static std::size_t PolyContainerSize(void) {
        return std::tuple_size_v<typename Container::self>;
    }

    // FindTupleType but for PolymorphicContainer
    template <typename T, typename Container, std::size_t Next = 0> constexpr static std::size_t FindPolyContainerSubType(void) {
        if constexpr (std::is_same_v<T, decltype(PolyContainerGet<Next>(std::declval<const Container>()))>) return Next;

        if constexpr (Next + 1 < Container::Size) return FindPolyContainerSubType<T, Container, Next + 1>();
        else return false;
    }

    // Loop over a fake polymorphic container.
    template <typename Container, typename Pred, std::size_t Next = 0> constexpr inline void PolyContainerForEach(Container& container, const Pred& pred) {
        for (auto& elem : PolyContainerGet<Next>(container)) pred(elem);
        if constexpr (Next + 1 < PolyContainerSize<Container>()) PolyContainerForEach<Container, Pred, Next + 1>(container, pred);
    }

    // Get the underlying subcontainer of a given type in the fake-polymorphic compound container.
    template <typename T, template <typename...> typename Wrapper, typename Container> constexpr inline auto PolySubContainerForT(Container& t) {
        if constexpr (std::is_same_v<Wrapper<T>, NoWrapperType<T>>) return PolyContainerGet<FindPolyContainerSubType<typename Container::template container<T>, Container>()>(t);
        else return PolyContainerGet<FindPolyContainerSubType<typename Container::template container<T>>, Container>()>(t);
    }
}