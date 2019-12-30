#pragma once

#include "Core.h"
#include "WindowController.h"
#include "Renderer2D.h"
#include "StaticPolymorphism.h"

#include "IEntity.h"
#include "EntityTank.h"
#include "EntityRocket.h"


#include <vector>
#include <string>
#include <algorithm>

#include "GLCompiler.h"
#include "GLBuffer.h"
#include "GLProgram.h"
#include "ILayoutObject.h"


namespace DroneSim::Game {
    class GameController {
        using Entities = Traits::DerivingClassList<IEntity, EntityTank, EntityRocket>;
    public:
        static GameController& instance(void) {
            static GameController instance { };
            return instance;
        }


        void start(std::vector<std::string>&& args) {
            this->args = std::move(args);

            // Force initialization.
            Render::WindowController::instance();
            Render::Renderer2D::instance();


            struct Vertex2D : public GPU::ILayoutObject<Vertex2D> {
                Vertex2D(Vec2f pos) : position(pos) {}

                Vec2f position;

                static std::vector<GPU::LayoutComponent> GetObjectLayout(void) {
                    return { DRONESIM_GEN_LAYOUT_OBJ(Vertex2D, position) };
                }
            };

            std::vector<Vertex2D> vertices = {
                { { 0.0f, 0.0f } }, { { 0.2f, 0.0f } }, { { 0.2f, 0.2f } },
                { { 0.0f, 0.4f } }, { { 0.2f, 0.4f } }, { { 0.2f, 0.6f } }
            };

            auto program = GPU::GLCompiler::instance().compile("test");
            auto buffer  = GPU::GLBuffer(GL_TRIANGLES, vertices);

            program.addBuffer(buffer);
            
            float t = 0;
            while (true) {
                Render::WindowController::instance().onFrameStart();
                
                program.setUniform(t, "t");
                program.execute();

                Render::WindowController::instance().onFrameEnd();

                t += 0.001;
            }

            loop();
        }


        template <typename T, typename = std::enable_if_t<Entities::Contains<T>()>> void addEntity(T&& entity) {
            Traits::PolyContainerGetT<T>(entities).push_back(std::move(entity));
        }


        template <typename T, typename = std::enable_if_t<Entities::Contains<T>()>> void removeEntity(const T& entity) {
            auto& v = Traits::PolyContainerGetT<T>(entities)
            Utility::swap_erase(v, std::find(v.begin(), v.end(), entity));
        }
    private:
        std::vector<std::string> args;
        u64 frames = 0;

        Entities::PolyContainer<std::vector> entities;


        void loop(void) {
            auto& window   = Render::WindowController::instance();
            auto& renderer = Render::Renderer2D::instance();

            while (!window.shouldClose()) {
                if (frames < 2000) {
                    // Progress simulation
                    Traits::PolyContainerForEach(entities, [](auto& entity) { entity.update(); });
                } else {
                    // Show results.
                }

                // Render the simulation.
                window.onFrameStart();
                renderer.renderFrame();
                window.onFrameEnd();

                ++frames;
            }
        }
    };
}