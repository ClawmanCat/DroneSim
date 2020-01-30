#pragma once

#include "Core.h"
#include "IEntity.h"


namespace DroneSim::Game {
    template <typename D> class EntityRotating : public IEntity<D> {
    public:
        using IEntity<D>::IEntity;
        using Base = IEntity<D>;


        EntityRotating(const Vec2f& position, const Vec2f& target) : Base(position), target(target) {}


        constexpr static u32 GetFramesPerAnim(void) { return D::GetFramesPerAnim(); }


        void update(void) {
            auto& self = static_cast<D&>(*this);

            // Set frame to match rotation and increment it every tick.
            const Vec2f absdir = glm::abs(self.getDirection());

            const Vec2f direction = self.getDirection();
            u32 frame_base = GetFramesPerAnim() * ((absdir.x > absdir.y)
                ? (direction.x < 0) ? 1 : 0
                : (direction.y < 0) ? 2 : 3);

            // If there are more frames in the current animation, go to the next frame, otherwise restart the animation.
            // (always restart if the animation has changed.)
            if (frame - frame_base < (GetFramesPerAnim() - 1)) ++frame;
            else frame = frame_base;
        }


        Vec2f getTarget(void)    const { return target; }
        Vec2f getDirection(void) const { return target - position; }
    protected:
        Vec2f target;
    };
}