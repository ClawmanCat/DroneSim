#pragma once

#include "Core.h"
#include "ILayoutObject.h"
#include "ITextureProvider.h"

#include <GL/glew.h>

namespace DroneSim::Game {
    template <typename D, typename Changes> class IEntity : public GPU::ILayoutObject<D>, public GPU::ITextureProvider<D> {
    public:
        // Can the contents of this object change?
        constexpr static bool changes = Changes::value;


        constexpr static Vec2f GetSize(void) {
            return D::GetSize();
        }

        constexpr static u32 GetFrameCount(void) {
            return D::GetFrameCount();
        }


        void update(void) {
            static_cast<D*>(this)->update();
        }

        Vec2f getPosition(void) const {
            return static_cast<D*>(this)->getPosition();
        }

        float getRotation(void) const {
            return static_cast<D*>(this)->getRotation();
        }

        u32 getTextureFrame(void) const {
            return static_cast<D*>(this)->getTextureFrame();
        }
    protected:
        IEntity(void) = default;
    };
}