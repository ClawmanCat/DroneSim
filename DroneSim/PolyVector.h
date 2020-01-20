#pragma once

#include "Core.h"
#include "StaticPolymorphism.h"

#include <vector>


namespace DroneSim::Traits {
    // Useful overloads for std::vector
    // std::vector<T>::f(args...) => PolyVector<>::f<T>(args...)
    template <template <typename...> typename Wrapper = NoWrapper, typename... Classes> 
    class PolyVector : public PolyContainer<std::vector, Wrapper, Classes...> {
    public:
        using Self = PolyContainer<std::vector, Wrapper, Classes...>;
        using PolyContainer<std::vector, Wrapper, Classes...>::PolyContainer;
        
        template <typename T> using vec_t = std::vector<typename Self::template wrapped_t<T>>;


        template <typename T>
        void push_back(const T& elem) { return static_cast<Self*>(this)->template getT<T>().push_back(elem); }


        template <typename T>
        void push_back(T&& elem) { return static_cast<Self*>(this)->template getT<T>().push_back(std::forward<T>(elem)); }


        template <typename T>
        void pop_back(void) { return static_cast<Self*>(this)->template getT<T>().pop_back(); }


        template <typename T>
        auto& at(std::size_t index) { return static_cast<Self*>(this)->template getT<T>().at(index); }

        template <typename T>
        const auto& at(std::size_t index) const { return static_cast<const Self*>(this)->template getT<T>().at(index); }


        template <typename T>
        auto& front(void) { return static_cast<Self*>(this)->template getT<T>().front(); }

        template <typename T>
        const auto& front(void) const { return static_cast<const Self*>(this)->template getT<T>().front(); }


        template <typename T>
        auto& back(void) { return static_cast<Self*>(this)->template getT<T>().back(); }

        template <typename T>
        const auto& back(void) const { return static_cast<const Self*>(this)->template getT<T>().back(); }


        template <typename T>
        auto begin(void) { return static_cast<Self*>(this)->template getT<T>().begin(); }

        template <typename T>
        auto cbegin(void) const { return static_cast<const Self*>(this)->template getT<T>().cbegin(); }


        template <typename T>
        auto end(void) { return static_cast<Self*>(this)->template getT<T>().end(); }

        template <typename T>
        auto cend(void) const { return static_cast<const Self*>(this)->template getT<T>().cend(); }


        template <typename T>
        std::size_t size(void) const { return static_cast<const Self*>(this)->template getT<T>().size(); }


        template <typename T>
        typename vec_t<T>::iterator erase(typename vec_t<T>::iterator pos) { return static_cast<Self*>(this)->template getT<T>().erase(pos); }

        template <typename T>
        typename vec_t<T>::iterator erase(typename vec_t<T>::iterator first, typename vec_t<T>::iterator last) { return static_cast<Self*>(this)->template getT<T>().erase(first, last); }


        template <typename T>
        typename vec_t<T>::iterator erase(typename vec_t<T>::const_iterator pos) { return static_cast<Self*>(this)->template getT<T>().erase(pos); }

        template <typename T>
        typename vec_t<T>::iterator erase(typename vec_t<T>::const_iterator first, typename vec_t<T>::const_iterator last) { return static_cast<Self*>(this)->template getT<T>().erase(first, last); }
    };
}