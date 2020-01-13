#pragma once

#include "Core.h"
#include "IEntity.h"


namespace DroneSim::Game {
    template <typename D> class EntityRotating : public IEntity<D> {
    public:
        using IEntity<D>::IEntity;


        constexpr static u32 GetFramesPerAnim(void) { return D::GetFramesPerAnim(); }


        void update(void) {
            // Cast to derived to prevent unqualified lookup into base.
            D& self = *static_cast<D*>(this);

            // Set frame to match rotation and increment it every tick.
            u32 frame_base = 0;

            if      (self.rotation < 0.25 * PI || self.rotation > 1.75 * PI) frame_base = 0 * GetFramesPerAnim();     // Right
            else if (self.rotation < 0.75 * PI                             ) frame_base = 2 * GetFramesPerAnim();     // Down
            else if (self.rotation < 1.25 * PI                             ) frame_base = 1 * GetFramesPerAnim();     // Right
            else                                                             frame_base = 3 * GetFramesPerAnim();     // Up

            // If there are more frames in the current animation, go to the next frame, otherwise restart the animation.
            // (always restart if the animation has changed.)
            if (self.frame - frame_base < (GetFramesPerAnim() - 1)) ++self.frame;
            else self.frame = frame_base;
        }
    };
}