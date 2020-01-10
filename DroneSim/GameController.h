#pragma once

#include "Core.h"
#include "WindowController.h"
#include "EntityList.h"
#include "GameConstants.h"
#include "Renderer2D.h"
#include "EntityTest.h"

#include <vector>
#include <string>
#include <algorithm>


namespace DroneSim::Game {
    class GameController {
    public:
        static GameController& instance(void) {
            static GameController instance { };
            return instance;
        }


        void start(std::vector<std::string>&& args) {
            this->args = std::move(args);

            /*auto program = GPU::GLCompiler::instance().compile("batch");
            
            std::vector data = {
                EntityTank<Team::BLUE>{ { 10.0f, 10.0f }, 0.0f }
            };

            auto buffer = GPU::GLBuffer(GL_POINTS, data);
            program.addBuffer(buffer);

            u32 frame = 0;
            auto& window = Render::WindowController::instance();
            while (!window.shouldClose()) {
                for (auto& elem : data) elem.update();
                buffer.modify(0, data);

                window.onFrameStart();


                program.setUniform(Vec2f{ 1.0 / 1280, 1.0 / 720 }, "window");
                program.setUniform(EntityTank<Team::RED>::GetSize(), "size");
                program.setUniform(EntityTank<Team::RED>::GetFrameCount(), "fCount");
                program.execute();

                window.onFrameEnd();

                if (++frame % 1000 == 0) std::cout << "Frame " << frame << '\n';
            }*/


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

        Render::Renderer2D renderer;


        GameController(void) : entities(GetInitialEntities()), renderer(entities) {}


        void loop(void) {
            auto& window = Render::WindowController::instance();

            while (!window.shouldClose()) {
                if (frames < 2000) {
                    // Progress simulation
                    Traits::PolyContainerForEach(entities, [](auto& entity, std::size_t n) { entity.update(); });

                    #ifndef NDEBUG
                        if (frames % 1000 == 0) std::cout << "Frame " << frames << '\n';
                    #endif
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


        static Entities::PolyContainer<std::vector> GetInitialEntities(void) {
            Entities::PolyContainer<std::vector> result;

            constexpr u32   tank_rows     = 12;
            constexpr float tank_spacing  = 15.0f;
            constexpr Vec2f corner_offset = EntityTank<Team::BLUE>::GetSize() + Vec2f{ 16.0f, 18.0f };


            auto place_color = [&](u32 count, auto& dest, const Vec2f& start, float rotation, const auto& op) {
                u32 r = 0, c = 0;

                for (u32 i = 0; i < count; ++i) {
                    dest.push_back({
                        Vec2f {
                            op(op(start.x, corner_offset.x), r * tank_spacing),
                            (start.y - corner_offset.y) - (c * tank_spacing)
                        }, 
                        rotation
                    });

                    if (++r > tank_rows) {
                        r = 0;
                        ++c;
                    }
                }
            };


            constexpr float w = WINDOW_WIDTH;
            constexpr float h = WINDOW_HEIGHT;

            place_color(BLUE_TANK_COUNT, Traits::PolyContainerGetT<EntityTank<Team::BLUE>>(result), Vec2f{ 0, h }, 0.0f, [](const auto& a, const auto& b) { return a + b; });
            place_color(RED_TANK_COUNT,  Traits::PolyContainerGetT<EntityTank<Team::RED>> (result), Vec2f{ w, h }, PI,   [](const auto& a, const auto& b) { return a - b; });

            
            return result;
        }
    };
}