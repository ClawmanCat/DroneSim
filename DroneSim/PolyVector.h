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


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        void push_back(T&& elem) { return static_cast<Self*>(this)->template getT<T>().push_back(std::forward<T>(elem)); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        void pop_back(void) { return static_cast<Self*>(this)->template getT<T>().pop_back(); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        T& at(std::size_t index) { return static_cast<Self*>(this)->template getT<T>().at(index); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        const T& at(std::size_t index) const { return static_cast<const Self*>(this)->template getT<T>().at(index); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        T& front(void) { return static_cast<Self*>(this)->template getT<T>().front(); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        const T& front(void) const { return static_cast<const Self*>(this)->template getT<T>().front(); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        T& back(void) { return static_cast<Self*>(this)->template getT<T>().back(); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        const T& back(void) const { return static_cast<const Self*>(this)->template getT<T>().back(); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        auto begin(void) { return static_cast<Self*>(this)->template getT<T>().begin(); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        auto cbegin(void) const { return static_cast<const Self*>(this)->template getT<T>().cbegin(); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        auto end(void) { return static_cast<Self*>(this)->template getT<T>().end(); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        auto cend(void) const { return static_cast<const Self*>(this)->template getT<T>().cend(); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        std::size_t size(void) const { return static_cast<const Self*>(this)->template getT<T>().size(); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        typename vec_t<T>::iterator erase(typename vec_t<T>::iterator pos) { return static_cast<Self*>(this)->template getT<T>().erase(pos); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        typename vec_t<T>::iterator erase(typename vec_t<T>::iterator first, typename vec_t<T>::iterator last) { return static_cast<Self*>(this)->template getT<T>().erase(first, last); }


        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        typename vec_t<T>::iterator erase(typename vec_t<T>::const_iterator pos) { return static_cast<Self*>(this)->template getT<T>().erase(pos); }

        template <typename T, typename = std::enable_if_t<Self::Subclasses::template Contains<T>()>>
        typename vec_t<T>::iterator erase(typename vec_t<T>::const_iterator first, typename vec_t<T>::const_iterator last) { return static_cast<Self*>(this)->template getT<T>().erase(first, last); }
    };
}