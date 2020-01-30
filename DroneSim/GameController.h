#pragma once

#include "Core.h"
#include "WindowController.h"
#include "EntityList.h"
#include "GameConstants.h"
#include "Renderer2D.h"
#include "Timer.h"
#include "PolyVector.h"
#include "EntitySelectors.h"
#include "PNGLoader.h"
#include "SpatialPartition.h"
#include "ConstexprFor.h"

#include <vector>
#include <string>
#include <algorithm>
#include <future>


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


        template <typename T, typename = std::enable_if_t<Entities::Contains<T>()>> void lockEntityStorage(void) {
            entitiesMtx[entities.FindT<T>()].lock();
        }


        template <typename T, typename = std::enable_if_t<Entities::Contains<T>()>> void unlockEntityStorage(void) {
            entitiesMtx[entities.FindT<T>()].unlock();
        }


        auto& getEntities(void) { return entities; }
        const auto& getEntities(void) const { return entities; }
        
        template <Team team> auto&       getPartitions(void)       { if constexpr (team == Team::BLUE) return *bluept; else return *redpt; }
        template <Team team> const auto& getPartitions(void) const { if constexpr (team == Team::BLUE) return *bluept; else return *redpt; }

        u32 getFrameCount(void) const { return frames; }
    private:
        constexpr static milliseconds reference_time = 73'520ms;
        Utility::Timer timer;

        std::vector<std::string> args;
        u64 frames = 0;

        Entities::PolyVector<> entities;
        std::array<std::mutex, decltype(entities)::Size> entitiesMtx;

        // Tank position partitioned storage.
        SpatialPartition<EntityTank<Team::BLUE>>* bluept;
        SpatialPartition<EntityTank<Team::RED >>* redpt;

        std::array<EntityChar*, 4> counter;     // Frame counter.

        Render::Renderer2D renderer;


        GameController(void) : timer(), entities(GetInitialEntities()), renderer(entities) {
            // Set frame counter.
            for (u32 i = 0; i < counter.size(); ++i) {
                counter[i] = &(*(entities.getT<EntityChar>().end() - counter.size() + i));
            }
        }


        ~GameController(void) {
            delete bluept;
            delete redpt;
        }


        void tick(void) {
            rebuild_partitions();

            mt_iterate<TankSelector>([](auto& tank) { tank.avoidCollision(); });

            // Update tanks and rockets asynchronously, update other entities on the main thread.
            // Don't update tanks and rockets at the same time, so we can safely push new rockets.
            // (some require GL calls which may only be done from the main thread.)
            mt_iterate<TankSelector>([](auto& entity) { entity.update(); });
            mt_iterate<RocketSelector>([](auto& entity) { entity.update(); });

            entities.select<Invert<Or<TankSelector, RocketSelector>>>().forEach([](auto& entity) { entity.update(); });

            mt_iterate<SelectAll>([](auto& entity) { entity.post_update(); });


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
            // Insertion sort is actually faster here since the array is already nearly sorted.
            entities.select<TankSelector>().forEachSub([](auto& v) {
                for (u32 i = 1; i < v.size(); ++i) {
                    auto current = std::move(v[i]);

                    i32 j;
                    for (j = i - 1; j >= 0 && v[j].getHealth() > current.getHealth(); --j) {
                        v[j + 1] = std::move(v[j]);
                    }

                    v[j + 1] = std::move(current);
                }
            });



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


        void rebuild_partitions(void) {
            delete bluept;
            delete redpt;

            redpt  = new SpatialPartition<EntityTank<Team::RED >>();
            bluept = new SpatialPartition<EntityTank<Team::BLUE>>();


            auto tanks = entities.select<TankSelector>();
            tanks.forEach([&](auto& tank) {
                using type = no_cref<decltype(tank)>;
                getPartitions<type::GetTeam()>().insert(&tank);
            });
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


        template <typename Selector, typename Pred> void mt_iterate(const Pred& pred) {
            std::vector<std::future<void>> handles;
            const u32 num_workers = 2 * std::thread::hardware_concurrency();

            entities.select<Selector>().forEachSub([&](auto& v) {
                u32 batch_size = (v.size() / num_workers) + 1;

                for (u32 i = 0; i < std::min(num_workers, (u32) v.size()); ++i) {
                    auto h = std::async(std::launch::async, [i, batch_size, pred, &v]() {
                        for (u32 j = batch_size * i; j < std::min(batch_size * (i + 1), (u32) v.size()); ++j) pred(v[j]);
                    });

                    handles.push_back(std::move(h));
                }
            });


            for (auto& handle : handles) handle.get();
        }
    };
}