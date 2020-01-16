#pragma once

#include "Core.h"
#include "WindowController.h"
#include "EntityList.h"
#include "GameConstants.h"
#include "Renderer2D.h"
#include "Timer.h"
#include "PolyVector.h"

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
        struct TankSelector {
            template <typename C, std::size_t N> constexpr static bool select(void) {
                return C::value_type::is_tank_tag;
            }
        };


        constexpr static milliseconds reference_time = 73'520ms;
        Utility::Timer timer;

        std::vector<std::string> args;
        u64 frames = 0;

        Entities::PolyVector<> entities;

        Render::Renderer2D renderer;


        GameController(void) : timer(), entities(GetInitialEntities()), renderer(entities) {}


        void tick(void) {
            // Perform collision detection on tanks
            // TODO: Proximity-friendly storage. (quad-tree or such)
            auto tanks = entities.select<TankSelector>();

            tanks.forEach([&](auto& tank, std::size_t ti) {
                tanks.forEach([&](auto& other, std::size_t oi) {
                    if (std::is_same_v<decltype(tank), decltype(other)> && ti == oi) return;

                    Vec2f direction  = tank.getPosition() - other.getPosition();
                    float distanceSq = glm::dot(direction, direction);
                    
                    float radiusSq = 2 * no_ref<decltype(tank)>::GetRadius();
                    radiusSq *= radiusSq;


                    if (distanceSq < radiusSq) {

                    }
                });
            });


            // Update entities.
            entities.forEach([](auto& entity, std::size_t n) { 
                entity.update(); 
            });


            // Remove finished explosions.
            entities.erase<EntityExplosion>(
                std::remove_if(
                    entities.begin<EntityExplosion>(), 
                    entities.end<EntityExplosion>(), // <-- TODO: Only search until last done explosion. (Vector is sorted)
                    [](const EntityExplosion& e) { return e.isFinished(); }
                ), 
                entities.end<EntityExplosion>()
            );
        }


        // Main simulation update loop.
        void loop(void) {
            auto& window = Render::WindowController::instance();

            while (!window.shouldClose()) {
                if (frames < SIMULATED_FRAMES) {
                    tick();

                    if (frames % 100 == 0) std::cout << "Frame " << frames << " (" << timer.elapsed().count() << "ms)" << "\n";
                } else {
                    // Show results.
                    if (frames == SIMULATED_FRAMES) {
                        milliseconds result = timer.elapsed();

                        std::cout << "Simulation took " << result.count() << "ms.\n";
                        std::cout << "% of original runtime: " << (100 * ((float) result.count()) / reference_time.count()) << "%\n";
                    }
                }

                // Render the simulation.
                window.onFrameStart();
                renderer.renderFrame();
                window.onFrameEnd();

                ++frames;
            }
        }


        static Entities::PolyVector<> GetInitialEntities(void) {
            Entities::PolyVector<> result;
            result.push_back<EntityBackground>(EntityBackground());

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

            place_color(BLUE_TANK_COUNT, result.getT<EntityTank<Team::BLUE>>(), Vec2f{ 0, h }, 0.0f, [](const auto& a, const auto& b) { return a + b; });
            place_color(RED_TANK_COUNT,  result.getT<EntityTank<Team::RED>> (), Vec2f{ w, h }, PI,   [](const auto& a, const auto& b) { return a - b; });

            
            return result;
        }
    };
}