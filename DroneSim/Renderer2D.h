#pragma once

#include "Core.h"
#include "StaticPolymorphism.h"
#include "EntityList.h"
#include "GLBuffer.h"
#include "GLCompiler.h"
#include "GLProgram.h"
#include "GameConstants.h"
#include "VariadicSplitter.h"
#include "ILayoutObject.h"
#include "HealthBar.h"
#include "GLFrameBuffer.h"
#include "EntitySelectors.h"

#include <vector>


namespace DroneSim::Render {
    class Renderer2D {
    public:
        Renderer2D(const Renderer2D&) = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;


        Renderer2D(const Game::Entities::PolyVector<>& entities, const std::vector<Game::HealthBar>& top, const std::vector<Game::HealthBar>& btm) :
            renderables(entities),
            buffers(entities.convert<decltype(buffers)>([](const auto& v) {
                return std::vector{ new GPU::GLBuffer<typename no_ref<decltype(v)>::value_type>(GL_POINTS) };
            })),
            worldShader(GPU::GLCompiler::instance().compile("batch")),
            healthShader(GPU::GLCompiler::instance().compile("health")),
            topbar(top),
            btmbar(btm)
        {}


        ~Renderer2D(void) {
            buffers.forEach([](auto ptr) { delete ptr; });
        }


        void renderFrame(void) {
            constexpr Vec2f WINDOW_SCALE = Vec2f{ 1.0f / Game::WINDOW_WIDTH, 1.0f / Game::WINDOW_HEIGHT };


            // Render world contents
            // Most of the buffer contents change each frame, so there's not much point in doing a partial update.
            buffers.forEach([&](auto ptr, std::size_t i, auto n) {
                auto& buffer = *ptr;

                const auto& v = renderables.get<decltype(n)::value>();
                
                buffer.modify(0, v);
                buffer.resize(v.size());

                // TODO: Find out why just adding the buffer once doesn't work.
                worldShader.addBuffer(ptr);

                worldShader.setUniform("window", WINDOW_SCALE                                         );
                worldShader.setUniform("size",   no_ref<decltype(buffer)>::value_type::GetSize()      );
                worldShader.setUniform("fCount", no_ref<decltype(buffer)>::value_type::GetFrameCount());

                worldShader.execute();

                worldShader.removeBuffer(ptr);
            });


            // Render health bars
            constexpr static float HEIGHT = 2.0f * float(Game::HEALTHBAR_HEIGHT) / Game::WINDOW_HEIGHT;

            auto renderbuffer = [&](const auto& src, auto& dest, float offset) {
                dest.modify(0, src);
                dest.resize(src.size());

                healthShader.addBuffer(&dest);

                healthShader.setUniform("count",  (u32) src.size());
                healthShader.setUniform("offset", offset);
                healthShader.setUniform("height", HEIGHT);
                healthShader.setUniform("limit",  (float) Game::TANK_MAX_HEALTH);

                healthShader.execute();

                healthShader.removeBuffer(&dest);
            };

            renderbuffer(topbar, topbuffer, 1.0 - HEIGHT);
            renderbuffer(btmbar, btmbuffer, -1.0);
        }
    private:
        // Store pointer so we can keep references to each buffer in the renderer.
        // TODO: Test performance with buffer pointers vs. updating buffer location before rendering.
        template <typename... Ts> using Wrapper = GPU::GLBuffer<Ts...>*;

        const Game::Entities::PolyVector<>& renderables;
        Game::Entities::Wrap<Wrapper>::PolyVector<> buffers;

        GPU::GLProgram worldShader, healthShader;


        // Health bars.
        const std::vector<Game::HealthBar> &topbar, &btmbar;
        GPU::GLBuffer<Game::HealthBar> topbuffer, btmbuffer;
    };
}