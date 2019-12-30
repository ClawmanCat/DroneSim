#include "EntityRocket.h"
#include "Renderer2D.h"

#include <limits>

namespace DroneSim::Game {
    constexpr static u32 INVALID_RENDER_ID = std::numeric_limits<u32>::max();


    EntityRocket::EntityRocket(Vec2f position, float rotation) :
        IEntity<EntityRocket>::IEntity(),
        IRenderable<EntityRocket>::IRenderable(),
        position(position),
        rotation(rotation),
        renderID(Render::Renderer2D::instance().add<EntityRocket>(*this).id)
    {}


    EntityRocket::EntityRocket(EntityRocket&& other) :
        IEntity<EntityRocket>::IEntity(std::move(other)),
        IRenderable<EntityRocket>::IRenderable(std::move(other)),
        position(other.position),
        rotation(other.rotation),
        renderID(other.renderID)
    {
        Render::Renderer2D::instance().update<EntityRocket>(renderID, *this);
        other.renderID = INVALID_RENDER_ID;
    }


    EntityRocket::~EntityRocket(void) {
        if (renderID != INVALID_RENDER_ID) Render::Renderer2D::instance().remove<EntityRocket>(renderID);
    }


    void EntityRocket::update(void) {

    }


    const Vec2f& EntityRocket::GetPosition(void) const {
        return position;
    }


    float EntityRocket::GetRotation(void) const {
        return rotation;
    }


    const Vec2f& EntityRocket::GetSize(void) const {
        const static Vec2f size = { WIDTH, HEIGHT };
        return size;
    }


    u8 EntityRocket::GetFrameCount(void) const {
        return FRAME_COUNT;
    }


    u16 EntityRocket::GetTextureID(u8 frame) const {
        return 0;   // TODO
    }
}