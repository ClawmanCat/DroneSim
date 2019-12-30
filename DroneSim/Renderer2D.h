#pragma once

#include "Core.h"
#include "RenderableList.h"
#include "VariadicSplitter.h"


#include <type_traits>
#include <vector>
#include <queue>

namespace DroneSim::Render {
    class Renderer2D {
    public:
        template <std::size_t N> struct RenderID {
            constexpr static std::size_t TypeID = N;

            RenderID(u32 id) : id(id) {}

            u32 id;
        };

    private:
        Renderables::PolyContainer<std::vector, PointerWrapper> renderables;

        // There is only one argument, but PolyContainer::container will fail to match if the declaration is not variadic.
        template <typename... Ts> using RenderIDStorage = RenderID<Traits::PolyContainerFindT<typename Traits::VariadicSplitter<Ts...>::head, decltype(renderables)>()>;
        Renderables::PolyContainer<std::queue, RenderIDStorage> freeIDs;

    public:
        // There is only one argument, but PolyContainer::container will fail to match if the declaration is not variadic.
        template <typename... Ts> using IDTypeFor = RenderIDStorage<std::remove_reference_t<typename Traits::VariadicSplitter<Ts...>::head>>;


        static Renderer2D& instance(void) {
            static Renderer2D instance { };
            return instance;
        }


        void renderFrame(void) {

        }


        template <typename T, typename = std::enable_if_t<Renderables::Contains<T>()>>
        IDTypeFor<T> add(const T& object) {
            auto& ids  = Traits::PolyContainerGetT<T>(freeIDs);
            auto& objs = Traits::PolyContainerGetT<T>(renderables);

            if (ids.size() > 0) {
                // Use an existing (dead) ID to decrease fragmentation.
                IDTypeFor<T> id = ids.front();
                ids.pop();

                objs[id.id] = &object;
                return id;
            } else {
                // Get a new ID.
                IDTypeFor<T> id = objs.size();

                objs.push_back(&object);
                return id;
            }
        }


        template <typename T, typename = std::enable_if_t<Renderables::Contains<T>()>>
        void remove(IDTypeFor<T> id) {
            auto& ids  = Traits::PolyContainerGet<decltype(id)::TypeID>(freeIDs);
            auto& objs = Traits::PolyContainerGet<decltype(id)::TypeID>(renderables);

            objs[id.id] = nullptr;
            ids.push(id);
        }


        // Since we keep a pointer to the object, we need to update the object pointed to when it is moved.
        template <typename T, typename = std::enable_if_t<Renderables::Contains<T>()>>
        void update(IDTypeFor<T> id, const T& newObject) {
            auto& objs = Traits::PolyContainerGet<decltype(id)::TypeID>(renderables);

            objs[id.id] = &newObject;
        }
    };
}