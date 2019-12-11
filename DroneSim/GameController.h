#pragma once

#include "Core.h"
#include "WindowController.h"
#include "StaticPolymorphism.h"

#include "IEntity.h"
#include "EntityTank.h"
#include "EntityRocket.h"


#include <vector>
#include <string>
#include <algorithm>

namespace DroneSim::Game {
    class GameController {
        using EntityList = Traits::DerivingClassList<IEntity, EntityTank, EntityRocket>;
    public:
        static GameController& instance(void) {
            static GameController instance { };
            return instance;
        }


        void start(std::vector<std::string>&& args) {
            this->args = std::move(args);

            addEntity(EntityTank());

            loop();
        }


        template <typename T, typename = std::enable_if_t<EntityList::Contains<std::remove_reference_t<T>>()>> void addEntity(T&& entity) {
            Traits::PolySubContainerForT<T, Traits::NoWrapperType>(entities).push_back(std::move(entity));
        }


        template <typename T, typename = std::enable_if_t<EntityList::Contains<std::remove_reference_t<T>>()>> void removeEntity(const T& entity) {
            auto& v = Traits::PolySubContainerForT<T, Traits::NoWrapperType>(entities);
            Utility::swap_erase(v, std::find(v.begin(), v.end(), entity));
        }
    private:
        std::vector<std::string> args;
        u64 frames = 0;

        EntityList::PolymorphicContainer<std::vector, Traits::NoWrapperType> entities;


        void loop(void) {
            auto& window = Render::WindowController::instance();

            while (!window.shouldClose()) {
                // Render the world.
                window.onFrameStart();

                if (frames < 2000) {
                    // Progress simulation
                    Traits::PolyContainerForEach(entities, [](auto& entity) { entity.update(); });
                } else {
                    // Show results.
                }

                window.onFrameEnd();

                ++frames;
            }
        }
    };
}