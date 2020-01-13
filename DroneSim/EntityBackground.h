#pragma once

#include "Core.h"
#include "IEntity.h"
#include "GameConstants.h"

// Not really an entity, but this saves us from having an extra base class just for this to derive from.
namespace DroneSim::Game {
    class EntityBackground : public IEntity<EntityBackground> {
    public:
        EntityBackground(void) : IEntity({ WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0 }, 0.0) {}

        constexpr static bool             MayChange(void)        { return false;                            }
        constexpr static Vec2f            GetSize(void)          { return { WINDOW_WIDTH, WINDOW_HEIGHT };  }
        constexpr static u32              GetFrameCount(void)    { return 1;                                }
        constexpr static std::string_view GetTexture(void)       { return "background";                     }


        void update(void) {
            // No need to update anything. Don't call superclass method.
        }
    };
}