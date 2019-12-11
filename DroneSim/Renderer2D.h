#pragma once

#include "Core.h"
#include "IRenderable.h"
#include "StaticPolymorphism.h"

#include <type_traits>
#include <vector>

namespace DroneSim::Render {
    class Renderer2D {
        template <typename T> using ConstRefWrapper = std::reference_wrapper<const T>;
        using Renderables = Traits::DerivingClassList<IRenderable>;
    public:
        static Renderer2D& instance(void) {
            static Renderer2D instance { };
            return instance;
        }


        template <typename T, typename = std::enable_if_t<Renderables::Contains<std::remove_reference_t<T>>()>>
        void add(const T& object) {
            Traits::PolySubContainerForT<T, ConstRefWrapper>(renderables).push_back(std::cref(object));
        }

        template <typename T, typename = std::enable_if_t<Renderables::Contains<std::remove_reference_t<T>>()>>
        void remove(const T& object) {
            auto& v = Traits::PolySubContainerForT<T, ConstRefWrapper>(renderables);
            Utility::swap_erase(v, std::find(v.begin(), v.end(), std::cref(object)));
        }
    private:
        // TODO: Test with using HashMap.
        Renderables::PolymorphicContainer<std::vector, ConstRefWrapper> renderables;
    };
}