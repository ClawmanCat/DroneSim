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


        template <typename T, typename = std::enable_if_t<Renderables::Contains<ConstRefWrapper<T>>()>>
        void add(const T& object) {
            Traits::PolyContainerGetT<T>(renderables).push_back(object);
        }

        template <typename T, typename = std::enable_if_t<Renderables::Contains<ConstRefWrapper<T>>()>>
        void remove(const T& object) {
            auto& container = Traits::PolyContainerGetT<T>(renderables);
            Utility::swap_erase(container, std::find(container.begin(), container.end(), object));
        }
    private:
        // TODO: Test with using HashSet for faster lookup when removing.
        Renderables::PolyContainer<std::vector, ConstRefWrapper> renderables;
    };
}