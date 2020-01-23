#pragma once

#include "Core.h"
#include "ILayoutObject.h"
#include "ITextureProvider.h"
#include "TextureManager.h"


namespace DroneSim::Game {
    struct HealthBar : public GPU::ILayoutObject<HealthBar>, public GPU::ITextureProvider<HealthBar> {
        u32 value;

        HealthBar(void) = default;
        HealthBar(u32 value) : value(value) {}


        constexpr static auto GetObjectLayout(void) {
            return std::array { DRONESIM_GEN_LAYOUT_OBJ(HealthBar, value) };
        }


        static GLuint GetTextureID(void) {
            static GLuint id = GPU::TextureManager::instance().getTextureID("healthbar");
            return id;
        }
    };
}