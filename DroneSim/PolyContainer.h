#pragma once

#include "Core.h"
#include "Pack.h"
#include "VariadicSplitter.h"
#include "ConstexprArg.h"
#include "TakesOverload.h"
#include "PolyContainerHelpers.h"

#include <tuple>
#include <functional>


namespace DroneSim::Traits {
    // TODO: Fix wrapper issues. (Move wrapping to ClassList?)
    template <
        template <typename...> typename Container, 
        template <typename...> typename Wrapper = NoWrapper,
        typename... Classes
    >
    class PolyContainer : public std::tuple<Container<WrappedType<Wrapper, Classes>>...> {
    public:
        using Subclasses = Pack<Classes...>;
        using Base = std::tuple<Container<WrappedType<Wrapper, Classes>>...>;
        template <typename... Ts> using container_t = Container<Ts...>;
        template <typename T>     using wrapped_t   = WrappedType<Wrapper, T>;

        using std::tuple<Container<WrappedType<Wrapper, Classes>>...>::tuple;


        PolyContainer(void) : Base() {}
        PolyContainer(const Base& b) : Base(b) {}
        PolyContainer(Base&& b) : Base(std::move(b)) {}


        // Gets the type of the Nth subcontainer.
        template <std::size_t N> using TypeAt = no_ref<decltype(std::get<N>(std::declval<Base>()))>;

        // Gets the element type of the Nth subcontainer.
        template <std::size_t N> using ElemTypeAt = typename no_ref<decltype(std::get<N>(std::declval<Base>()))>::value_type;

        // The number of subcontainers in this polycontainer.
        constexpr static std::size_t Size = std::tuple_size_v<Base>;



        // Find the index of the subcontainer containing elements of the type T.
        template <typename T, std::size_t N = 0> constexpr static std::size_t FindT(void) {
            if (std::is_same_v<wrapped_t<T>, ElemTypeAt<N>>) return N;
            else return FindT<T, N + 1>();
        }


        // Get the Nth subcontainer from this polycontainer.
        template <std::size_t N> constexpr auto& get(void) {
            return std::get<N>(*static_cast<Base*>(this));
        }


        // Get the Nth subcontainer from this polycontainer. (const)
        template <std::size_t N> constexpr const auto& get(void) const {
            return std::get<N>(*static_cast<const Base*>(this));
        }


        // Get the subcontainer containing elements with the type T.
        template <typename T, std::size_t N = 0> constexpr auto& getT(void) {
            if constexpr (std::is_same_v<wrapped_t<T>, ElemTypeAt<N>>) return get<N>();
            else return getT<T, N + 1>();
        }


        // Get the subcontainer containing elements with the type T. (const)
        template <typename T, std::size_t N = 0> constexpr const auto& getT(void) const {
            if constexpr (std::is_same_v<wrapped_t<T>, ElemTypeAt<N>>) return get<N>();
            else return getT<T, N + 1>();
        }


        // Convert this polycontainer to a new one, by applying pred to each subcontainer.
        template <typename Out, typename Pred> constexpr Out convert(const Pred& pred) const {
            if constexpr (Size == 0) return Out();
            else return Out(convertImpl(pred));
        }


        // Iterate over each element in each subcontainer. 
        // Signature of predicate should be void(auto& elem, [optional std::size_t index], [optional ConstexprArg<std::size_t, ?> subcontainer])
        template <typename Pred, std::size_t N = 0> constexpr void forEach(const Pred& pred) {
            if constexpr (N + 1 <= Size) {
                auto& sub = get<N>();
                
                for (std::size_t i = 0; i < sub.size(); ++i) {
                    // Pick correct overload
                    if constexpr (takes_overload_v<Pred, typename no_ref<decltype(sub)>::value_type&>) pred(sub[i]);
                    if constexpr (takes_overload_v<Pred, typename no_ref<decltype(sub)>::value_type&, std::size_t>) pred(sub[i], i);
                    if constexpr (takes_overload_v<Pred, typename no_ref<decltype(sub)>::value_type&, ConstexprArg<std::size_t, N>>) pred(sub[i], ConstexprArg<std::size_t, N>());
                    if constexpr (takes_overload_v<Pred, typename no_ref<decltype(sub)>::value_type&, std::size_t, ConstexprArg<std::size_t, N>>) pred(sub[i], i, ConstexprArg<std::size_t, N>());
                }
            }

            if constexpr (N + 1 < Size) forEach<Pred, N + 1>(pred);
        }


        // Iterate over each element in each subcontainer. (const)
        // Signature of predicate should be void(const auto& elem, [optional std::size_t index], [optional ConstexprArg<std::size_t, ?> subcontainer])
        template <typename Pred, std::size_t N = 0> constexpr void forEach(const Pred& pred) const {
            if constexpr (N + 1 <= Size) {
                const auto& sub = get<N>();
                
                for (std::size_t i = 0; i < sub.size(); ++i) {
                    // Pick correct overload
                    if constexpr (takes_overload_v<Pred, const typename no_ref<decltype(sub)>::value_type&>) pred(sub[i]);
                    if constexpr (takes_overload_v<Pred, const typename no_ref<decltype(sub)>::value_type&, std::size_t>) pred(sub[i], i);
                    if constexpr (takes_overload_v<Pred, const typename no_ref<decltype(sub)>::value_type&, ConstexprArg<std::size_t, N>>) pred(sub[i], ConstexprArg<std::size_t, N>());
                    if constexpr (takes_overload_v<Pred, const typename no_ref<decltype(sub)>::value_type&, std::size_t, ConstexprArg<std::size_t, N>>) pred(sub[i], i, ConstexprArg<std::size_t, N>());
                }
            }

            if constexpr (N + 1 < Size) forEach<Pred, N + 1>(pred);
        }


        // Iterate over each subcontainer.
        // Signature of predicate should be void(auto& subcontainer, [optional ConstexprArg<std::size_t, ?> index])
        template <typename Pred, std::size_t N = 0> constexpr void forEachSub(const Pred& pred) {
            if constexpr (N + 1 <= Size) {
                if constexpr (takes_overload_v<Pred, TypeAt<N>&>) pred(get<N>());
                if constexpr (takes_overload_v<Pred, TypeAt<N>&, ConstexprArg<std::size_t, N>>) pred(get<N>(), ConstexprArg<std::size_t, N>());
            }

            if constexpr (N + 1 < Size) forEachSub<Pred, N + 1>(pred);
        }


        // Iterate over each subcontainer. (const)
        // Signature of predicate should be void(const auto& subcontainer, [optional ConstexprArg<std::size_t, ?> index])
        template <typename Pred, std::size_t N = 0> constexpr void forEachSub(const Pred& pred) const {
            if constexpr (N + 1 <= Size) {
                if constexpr (takes_overload_v<Pred, const TypeAt<N>&>) pred(get<N>());
                if constexpr (takes_overload_v<Pred, const TypeAt<N>&, ConstexprArg<std::size_t, N>>) pred(get<N>(), ConstexprArg<std::size_t, N>());
            }

            if constexpr (N + 1 < Size) forEachSub<Pred, N + 1>(pred);
        }


        // Create a new PolyContainer, where each subcontainer is included only if Pred returns true.
        // This method only creates a new view on the current container, it does not copy the elements.
        // The predicate class must have a constexpr static method with signature 
        // template <typename container_t, std::size_t index> bool select(void)
        template <typename Pred, template <typename...> typename CC = Container, template <typename...> typename WW = Wrapper> constexpr auto select(void) {
            auto t = selectImpl<Pred>();

            using container_t = typename TupleInfo<decltype(t)>::type
                ::template Apply<RemoveReference>::type
                ::template Apply<ToValueType>::type
                ::template Apply<ToRefContainer>::type;

            return container_t(std::move(t));
        }


        // Create a new PolyContainer, where each subcontainer is included only if Pred returns true. (const)
        // This method only creates a new view on the current container, it does not copy the elements.
        // The predicate class must have a constexpr static method with signature 
        // template <typename container_t, std::size_t index> bool select(void)
        template <typename Pred> constexpr auto select(void) const {
            auto t = selectImpl<Pred>();

            using container_t = typename TupleInfo<decltype(t)>::type
                ::template Apply<RemoveReference>::type
                ::template Apply<ToValueType>::type
                ::template Apply<ToCRefContainer>::type;

            return container_t(std::move(t));
        }
    private:
        // Implementation details for PolyContainer::convert:
        template <typename Pred, std::size_t N = 0> constexpr auto convertImpl(const Pred& pred) const {
            auto call_pred = [&]() constexpr {
                if constexpr (takes_overload_v<Pred, decltype(get<N>())>) return pred(get<N>());
                if constexpr (takes_overload_v<Pred, decltype(get<N>()), ConstexprArg<std::size_t, N>>) return pred(get<N>(), ConstexprArg<std::size_t, N>());
            };

            if constexpr (Size == 0) return std::tuple { };
            else if constexpr (N + 1 == Size) return std::tuple{ call_pred() };
            else return std::tuple_cat(std::tuple{ call_pred() }, convertImpl<Pred, N + 1>(pred));
        }


        // Implementation details for PolyContainer::select:
        template <typename> struct TupleInfo {};
        template <typename... Xs> struct TupleInfo<std::tuple<Xs...>> { using type = Pack<Xs...>; };

        template <typename... Xs> using RemoveReference    = Pack<std::remove_reference_t<Xs>...>;
        template <typename... Xs> using ToValueType        = Pack<typename Xs::value_type...>;
        template <typename... Xs> using ToRefSubcontainer  = Container<Xs...>&;
        template <typename... Xs> using ToCRefSubcontainer = const Container<Xs...>&;

        template <typename... Xs> using ToRefContainer  = PolyContainer<ToRefSubcontainer,  Wrapper, Xs...>;
        template <typename... Xs> using ToCRefContainer = PolyContainer<ToCRefSubcontainer, Wrapper, Xs...>;


        template <typename Pred, std::size_t N = 0> constexpr auto selectImpl(void) {
            auto call_pred = [&]() constexpr {
                if constexpr (Pred::template select<TypeAt<N>, N>()) return std::tie(get<N>());
                else return std::tuple{};
            };

            if constexpr (Size == 0) return std::tuple{};
            else if constexpr (N + 1 == Size) return call_pred();
            else return std::tuple_cat(call_pred(), selectImpl<Pred, N + 1>());
        }


        template <typename Pred, std::size_t N = 0> constexpr auto selectImpl(void) const {
            auto call_pred = [&]() constexpr {
                if constexpr (Pred::template select<TypeAt<N>, N>()) return std::tie(get<N>());
                else return std::tuple{};
            };

            if constexpr (Size == 0) return std::tuple{};
            else if constexpr (N + 1 == Size) return call_pred();
            else return std::tuple_cat(call_pred(), selectImpl<Pred, N + 1>());
        }
    };
}