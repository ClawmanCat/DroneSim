#pragma once

#include "Core.h"
#include "StaticPolymorphism.h"
#include "IRenderable.h"
#include "EntityTank.h"
#include "EntityRocket.h"


namespace DroneSim::Render {
    using Renderables = Traits::DerivingClassList<IRenderable, Game::EntityTank, Game::EntityRocket>;


    template <typename T> struct PointerWrapper {
        const std::remove_reference_t<T>* ptr;

        PointerWrapper(void) = default;
        PointerWrapper(decltype(ptr) ptr) : ptr(ptr) {}
        operator decltype(ptr)(void) const{ return ptr; }
    };
}