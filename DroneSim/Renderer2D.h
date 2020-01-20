#pragma once

#include "Core.h"
#include "StaticPolymorphism.h"
#include "EntityList.h"
#include "GLBuffer.h"
#include "GLCompiler.h"
#include "GLProgram.h"
#include "GameConstants.h"
#include "VariadicSplitter.h"

#include <vector>


namespace DroneSim::Render {
    class Renderer2D {
    public:
        Renderer2D(const Renderer2D&) = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;


        Renderer2D(const Game::Entities::PolyVector<>& entities) : 
            renderables(entities),
            buffers(entities.convert<decltype(buffers)>([](const auto& v) {
                return std::vector{ new GPU::GLBuffer<typename no_ref<decltype(v)>::value_type>(GL_POINTS) };
            })),
            shader(GPU::GLCompiler::instance().compile("batch"))
        {
            //buffers.forEach([this](auto ptr, std::size_t i, auto n) {
            //    auto& buffer = *ptr;
            //
            //    // If the buffer doesn't change, we upload it once. (now)
            //    const auto& v = renderables.get<decltype(n)::value>();
            //
            //    buffer.modify(0, v);
            //    buffer.resize(v.size());
            //
            //    shader.addBuffer(ptr);
            //});
        }


        ~Renderer2D(void) {
            buffers.forEach([](auto ptr) { delete ptr; });
        }


        void renderFrame(void) {
            const static Vec2f WINDOW_SCALE = Vec2f{ 1.0f / Game::WINDOW_WIDTH, 1.0f / Game::WINDOW_HEIGHT };

            // Most of the buffer contents change each frame, so there's not much point in doing a partial update.
            buffers.forEach([this](auto ptr, std::size_t i, auto n) {
                auto& buffer = *ptr;

                const auto& v = renderables.get<decltype(n)::value>();
                
                // if constexpr (no_cref<decltype(v)>::value_type::MayChange()) {
                    buffer.modify(0, v);
                    buffer.resize(v.size());
                // }
                //if (v.size() == 0) return;

                // TODO: Find out why just adding the buffer once doesn't work.
                shader.addBuffer(ptr);

                shader.setUniform("window", WINDOW_SCALE                                         );
                shader.setUniform("size",   no_ref<decltype(buffer)>::value_type::GetSize()      );
                shader.setUniform("fCount", no_ref<decltype(buffer)>::value_type::GetFrameCount());

                shader.execute();

                shader.removeBuffer(ptr);
            });
        }
    private:
        // Store pointer so we can keep references to each buffer in the renderer.
        // TODO: Test performance with buffer pointers vs. updating buffer location before rendering.
        template <typename... Ts> using Wrapper = GPU::GLBuffer<Ts...>*;

        const Game::Entities::PolyVector<>& renderables;
        Game::Entities::Wrap<Wrapper>::PolyVector<> buffers;

        GPU::GLProgram shader;
    };
}