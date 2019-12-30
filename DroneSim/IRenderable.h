#pragma once

#include "Core.h"

namespace DroneSim::Render {
    template <typename D> class IRenderable {
    public:
        const Vec2f& GetPosition(void) const {
            return static_cast<D>(*this).GetPosition();
        }

        float GetRotation(void) const {
            return static_cast<D>(*this).GetRotation();
        }

        const Vec2f& GetSize(void) const {
            return static_cast<D>(*this).GetSize();
        }

        u8 GetFrameCount(void) const {
            return static_cast<D>(*this).GetFrameCount();
        }

        u16 GetTextureID(u8 frame) const {
            return static_cast<D>(*this).GetTextureID(frame);
        }
    protected:
        IRenderable(void) = default;
    };
}