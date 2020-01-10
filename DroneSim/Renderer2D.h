#pragma once

#include "Core.h"
#include "StaticPolymorphism.h"
#include "EntityList.h"
#include "GLBuffer.h"
#include "GLCompiler.h"
#include "GLProgram.h"
#include "GameConstants.h"

#include <vector>


namespace DroneSim::Render {
    class Renderer2D {
    public:
        using RenderID = u32;


        Renderer2D(const Game::Entities::PolyContainer<std::vector>& entities) : 
            renderables(entities),
            buffers(Traits::PolyContainerConvertingConstructor<no_ref<decltype(entities)>, no_ref<decltype(buffers)>>(entities, [](const auto& v, std::size_t n) {
                return GPU::GLBuffer<typename no_ref<decltype(v)>::value_type>(GL_POINTS, v.size());
            })),
            shader(GPU::GLCompiler::instance().compile("batch"))
        {
            Traits::PolyContainerForEachSub(buffers, [this](auto& buffer, auto n) {
                // If the buffer doesn't change, we upload it once. (now)
                if constexpr (!no_cref<decltype(Traits::PolyContainerGet<decltype(n)::value>(renderables))>::value_type::changes) {
                    const auto& v = Traits::PolyContainerGet<decltype(n)::value>(renderables);
                    if (v.size() > 0) buffer.modify(0, v);
                }
            });
        }


        void renderFrame(void) {
            const static Vec2f WINDOW_SCALE = Vec2f{ 1.0f / Game::WINDOW_WIDTH, 1.0f / Game::WINDOW_HEIGHT };

            // Most of the buffer contents change each frame, so there's not much point in doing a partial update.
            Traits::PolyContainerForEachSub(buffers, [this](auto& buffer, auto n) {
                const auto& v = Traits::PolyContainerGet<decltype(n)::value>(renderables);
                
                if constexpr (no_cref<decltype(v)>::value_type::changes) buffer.modify(0, v);
                if (v.size() == 0) return;


                auto id = shader.addBuffer(buffer);

                shader.setUniform(WINDOW_SCALE,                                          "window");
                shader.setUniform(no_ref<decltype(buffer)>::value_type::GetSize(),       "size"  );
                shader.setUniform(no_ref<decltype(buffer)>::value_type::GetFrameCount(), "fCount");

                shader.execute();

                shader.removeBuffer(id);
            });
        }
    private:
        const Game::Entities::PolyContainer<std::vector>& renderables;
        Game::Entities::PolyContainer<GPU::GLBuffer> buffers;

        GPU::GLProgram shader;
    };
}