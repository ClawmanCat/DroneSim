#include "EntityTank.h"
#include "Renderer2D.h"

#include <limits>

namespace DroneSim::Game {
    constexpr static u32 INVALID_RENDER_ID = std::numeric_limits<u32>::max();


    EntityTank::EntityTank(Vec2f position, float rotation) : 
        IEntity<EntityTank>::IEntity(),
        IRenderable<EntityTank>::IRenderable(),
        position(position), 
        rotation(rotation),
        renderID(Render::Renderer2D::instance().add<EntityTank>(*this).id)
    {}


    EntityTank::EntityTank(EntityTank&& other) :
        IEntity<EntityTank>::IEntity(std::move(other)),
        IRenderable<EntityTank>::IRenderable(std::move(other)),
        position(other.position),
        rotation(other.rotation),
        renderID(other.renderID)
    {
        Render::Renderer2D::instance().update<EntityTank>(renderID, *this);
        other.renderID = INVALID_RENDER_ID;
    }


    EntityTank::~EntityTank(void) {
        if (renderID != INVALID_RENDER_ID) Render::Renderer2D::instance().remove<EntityTank>(renderID);
    }


    void EntityTank::update(void) {

    }


    const Vec2f& EntityTank::GetPosition(void) const {
        return position;
    }


    float EntityTank::GetRotation(void) const {
        return rotation;
    }


    const Vec2f& EntityTank::GetSize(void) const {
        const static Vec2f size = { WIDTH, HEIGHT };
        return size;
    }


    u8 EntityTank::GetFrameCount(void) const {
        return FRAME_COUNT;
    }


    u16 EntityTank::GetTextureID(u8 frame) const {
        return 0; // TODO
    }
}