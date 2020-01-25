#pragma once

#include "Core.h"
#include "WindowController.h"
#include "EntityList.h"
#include "GameConstants.h"
#include "Renderer2D.h"
#include "Timer.h"
#include "PolyVector.h"
#include "EntitySelectors.h"
#include "HealthBar.h"
#include "PNGLoader.h"

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
            auto& v = Traits::PolyContainerGetT<T>(entities);
            
            std::size_t index = &entity - &v.data[0];
            Utility::swap_erase(v, v.begin() + index);
        }


        template <typename Tank> const auto& findClosestEnemy(const Tank& tank) const {
            const auto& enemies = entities.select<EnemyTankSelector<Tank::GetTeam()>>().template get<0>();

            std::size_t closest = 0;
            float distanceSq = std::numeric_limits<float>::infinity();

            for (std::size_t i = 0; i < enemies.size(); ++i) {
                if (!enemies[i].alive()) continue;

                Vec2f d = tank.getPosition() - enemies[i].getPosition();
                float dsq = Utility::dotself(d);

                if (dsq < distanceSq) {
                    closest = i;
                    distanceSq = dsq;
                }
            }

            return enemies[closest];
        };


        auto& getEntities(void) { return entities; }
        const auto& getEntities(void) const { return entities; }

        u32 getFrameCount(void) const { return frames; }
    private:
        constexpr static milliseconds reference_time = 73'520ms;
        Utility::Timer timer;

        std::vector<std::string> args;
        u64 frames = 0;

        Entities::PolyVector<> entities;
        std::vector<HealthBar> topbar, btmbar; // Health bars.
        std::array<EntityChar*, 4> counter;

        Render::Renderer2D renderer;


        GameController(void) : timer(), entities(GetInitialEntities()), renderer(entities, topbar, btmbar) {
            // Set frame counter.
            for (u32 i = 0; i < counter.size(); ++i) {
                counter[i] = &(*(entities.getT<EntityChar>().end() - counter.size() + i));
            }
        }


        void tick(void) {
            // Perform collision detection on tanks.
            auto tanks = entities.select<TankSelector>();
            tanks.forEach([](auto& tank) { tank.avoidCollision(); });


            // Update entities.
            entities.forEach([](auto& entity, std::size_t n) { 
                entity.update(); 
            });


            // Remove finished explosions.
            // TODO: Move to EntityExplosion.cpp
            entities.erase<EntityExplosion>(
                std::remove_if(
                    entities.begin<EntityExplosion>(), 
                    entities.end<EntityExplosion>(), // <-- TODO: Only search until last done explosion. (Vector is sorted)
                    [](const EntityExplosion& e) { return e.isFinished(); }
                ), 
                entities.end<EntityExplosion>()
            );
            
            
            // Remove exploded rockets.
            // TODO: Move to EntityRocket.cpp
            auto rockets = entities.select<RocketSelector>();
            rockets.forEachSub([](auto& container) {
                container.erase(
                    std::remove_if(
                        container.begin(),
                        container.end(),
                        [](const auto& rocket) { return !rocket.alive(); }
                    ),
                    container.end()
                );
            });


            // Calculate health bars.
            // TODO: std::sort
            auto healthsort = [](const auto& source, auto& dest) {
                auto convert = [](const auto& tank) { return HealthBar{ tank.getHealth() }; };
                
                dest.clear();
                dest.reserve(source.size());

                dest.push_back(convert(source.front()));

                for (u32 i = 1; i < source.size(); ++i) {
                    auto first = convert(source[i]);

                    for (u32 j = dest.size() - 1; j >= 0; --j) {
                        const auto& second = dest[j];

                        if (second.value > first.value) {
                            dest.insert(dest.begin() + j + 1, first);
                            break;
                        }

                        if (j == 0) {
                            dest.insert(dest.begin(), first);
                            break;
                        }
                    }
                }
            };
            

            healthsort(entities.getT<EntityTank<Team::BLUE>>(), topbar);
            healthsort(entities.getT<EntityTank<Team::RED >>(), btmbar);


            // Update frame counter.
            std::string chars = std::to_string(frames + 1); // +1 to go from [0, 1999] to [1, 2000].
            while (chars.length() < 4) chars = '0' + chars;

            for (u32 i = 0; i < counter.size(); ++i) counter[i]->set(chars[i]);
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
            result.push_back(EntityBackground());
            result.push_back(EntityBGOverlay());

            constexpr u32   tank_rows     = 12;
            constexpr float tank_spacing  = 15.0f;
            constexpr Vec2f tank_offset   = Vec2f{ 17.0f, WINDOW_HEIGHT - HEALTHBAR_HEIGHT - 18.0f };


            auto place_color = [&](u32 count, auto& dest, const Vec2f& target, bool invert, float side_offset) {
                dest.reserve(count);

                const Vec2f begin     = invert ? Vec2f{ WINDOW_WIDTH - tank_offset.x, tank_offset.y } - Vec2f{ side_offset, 0.0f } : tank_offset + Vec2f{ side_offset, 0.0f };
                const Vec2f direction = invert ? Vec2f{ -1.0, -1.0 } : Vec2f{ 1.0, -1.0 };

                u32 r = 0, c = 0;

                for (u32 i = 0; i < count; ++i) {
                    dest.push_back({
                        begin + (Vec2f{ r, c } * tank_spacing * direction),
                        target
                    });

                    if (++r > tank_rows) {
                        r = 0;
                        ++c;
                    }
                }
            };


            place_color(BLUE_TANK_COUNT, result.getT<EntityTank<Team::BLUE>>(), BLUE_TANK_DEFAULT_TARGET, false, 0.0f  );
            place_color(RED_TANK_COUNT,  result.getT<EntityTank<Team::RED>> (), RED_TANK_DEFAULT_TARGET,  true,  135.0f);


            auto& beams = result.getT<EntityBeam>();
            beams.push_back(EntityBeam(BLUE_TANK_DEFAULT_TARGET));
            beams.push_back(EntityBeam(RED_TANK_DEFAULT_TARGET));
            beams.push_back(EntityBeam({ WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0 }));

            
            auto  text  = EntityChar::MakeText({ WINDOW_WIDTH / 2.0 - (5 * EntityChar::KERNING), 100 }, "FRAME: 0000");
            auto& chars = result.getT<EntityChar>();

            std::move(text.begin(), text.end(), std::back_inserter(chars));


            return result;
        }
    };
}