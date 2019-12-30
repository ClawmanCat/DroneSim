#pragma once

#include "Core.h"

#include <type_traits>
#include <tuple>

namespace DroneSim::Traits {
    template <typename T> struct NoWrapper {};


    // Can be used for setting the return type of a function which can later be decltype'd, unlike std::declval.
    // (std::declval breaks with libc++ due to a static assert.)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wreturn-type"
    template <typename T> constexpr static T Declval(void) {};
    #pragma clang diagnostic pop


    // The return type of this function is Wrapper<T>, unless Wrapper is NoWrapper, then the return type is T.
    template <typename T, template <typename...> typename Wrapper> constexpr static auto GetWrappedType(void) {
        if constexpr (std::is_same_v<Wrapper<T>, NoWrapper<T>>) return Declval<T>();
        else return Declval<Wrapper<T>>();
    }


    // A list of classes with the given CRTP base class.
    template <template <typename> typename CRTPBase, typename... Derived> class DerivingClassList {
        public:
            // Produce a container for the given derived classes that fakes polymorphism at compile time.
            template <template <typename...> typename Container, template <typename...> typename Wrapper = NoWrapper>
            class PolyContainer : public std::tuple<Container<decltype(GetWrappedType<Derived, Wrapper>())>...> {
            
            public:
                typedef std::tuple<Container<decltype(GetWrappedType<Derived, Wrapper>())>...> self;
                
                // Can't use self::tuple since it requires 'typename' with clang-cl (and clang++) but not with MSVC.
                // (And there is no preprocessor-define that can seperate the two.)
                using std::tuple<Container<decltype(GetWrappedType<Derived, Wrapper>())>...>::tuple;

                template <typename... Ts> using container = Container<Ts...>;
                template <typename... Ts> using wrapper   = Wrapper<Ts...>;

                template <typename T> using contained_type = Container<decltype(GetWrappedType<T, Wrapper>())>;
            };


            // Is T one of the types in Derived?
            template <typename T> constexpr static bool Contains(void) {
                return ContainsImpl<T, Derived...>();
            }
        private:
            // Implementation seperate to split the variadic template.
            template <typename T, typename Head, typename... Tail> constexpr static bool ContainsImpl(void) {
                if constexpr (std::is_same_v<T, Head>) return true;

                if constexpr (sizeof...(Tail) == 0) return false;
                else return ContainsImpl<T, Tail...>();
            }
    };


    // Get the number of subcontainers of a PolyContainer.
    template <typename Container> constexpr static std::size_t PolyContainerSize(void) {
        return std::tuple_size_v<typename Container::self>;
    }


    // Get the Nth element of a PolyContainer. (Usage same as std::get)
    template <std::size_t N, typename Container> constexpr static auto& PolyContainerGet(Container& container) {
        return std::get<N>(static_cast<typename Container::self&>(container));
    }

    template <std::size_t N, typename Container> constexpr static const auto& PolyContainerGet(const Container& container) {
        return std::get<N>(static_cast<const typename Container::self&>(container));
    }


    // Get the index of the PolyContainer that stores elements of the type T.
    template <typename T, typename Container, std::size_t Next = 0> constexpr static std::size_t PolyContainerFindT(void) {
        if constexpr (std::is_same_v<
            typename Container::template contained_type<T>,
            std::tuple_element_t<Next, typename Container::self>
        >) return Next;

        else return PolyContainerFindT<T, Container, Next + 1>();
    }


    // Get the subcontainer of the PolyContainer that stores elements of the type T.
    template <typename T, typename Container> constexpr static auto& PolyContainerGetT(Container& container) {
        return PolyContainerGet<PolyContainerFindT<T, Container>()>(container);
    }

    template <typename T, typename Container> constexpr static const auto& PolyContainerGetT(const Container& container) {
        return PolyContainerGet<PolyContainerFindT<T, Container>()>(container);
    }


    // Iterate over the PolyContainer.
    template <typename Container, typename Pred, std::size_t Next = 0> constexpr static void PolyContainerForEach(Container& container, const Pred& pred) {
        if constexpr (Next < PolyContainerSize<Container>()) {
            for (auto& elem : PolyContainerGet<Next>(container)) pred(elem);
            PolyContainerForEach<Container, Pred, Next + 1>(container, pred);
        }
    }


    // Iterator over the subcontainers.
    template <typename Container, typename Pred, std::size_t Next = 0> constexpr static void PolyContainerForEachSub(Container& container, const Pred& pred) {
        if constexpr (Next < PolyContainerSize<Container>()) {
            pred(PolyContainerGet<Next>(container));
            PolyContainerForEachSub<Container, Pred, Next + 1>(container, pred);
        }
    }
}