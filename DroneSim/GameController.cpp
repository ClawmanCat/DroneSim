#include "GameController.h"


namespace DroneSim::Game {
    GameController& GameController::instance(void) {
        static GameController instance{ };
        return instance;
    }




    std::mutex& GameController::getEntitiesMtx(void) {
        static std::mutex mtx{};
        return mtx;
    }




    GameController::GameController(void) : timer(), entities(GetInitialEntities()), renderer(entities) {
        // Set frame counter.
        for (u32 i = 0; i < counter.size(); ++i) {
            counter[i] = &(*(entities.getT<EntityChar>().end() - counter.size() + i));
        }
    }
    


    
    GameController::~GameController(void) {
        delete bluept;
        delete redpt;
    }




    void GameController::start(std::vector<std::string>&& args) {
        this->args = std::move(args);
        loop();
    }




    // Main simulation update loop.
    void GameController::loop(void) {
        auto& window = Render::WindowController::instance();

        while (!window.shouldClose()) {
            if (frames < SIMULATED_FRAMES) {
                tick();

                if (frames % 100 == 0) std::cout << "Frame " << frames << " (" << timer.elapsed().count() << "ms)" << "\n";
            }
            else {
                // Show results.
                if (frames == SIMULATED_FRAMES) {
                    milliseconds result = timer.elapsed();

                    std::cout << "Simulation took " << result.count() << "ms.\n";
                    std::cout << "% of original runtime: " << (100 * ((float)result.count()) / reference_time.count()) << "%\n";
                }
            }

            // Render the simulation.
            window.onFrameStart();
            renderer.renderFrame();
            window.onFrameEnd();

            ++frames;
        }
    }




    void GameController::tick(void) {
        rebuild_partitions();

        mt_iterate<Not<Select<EntityBGOverlay>>>([](auto& entity) { entity.update(); });

        mt_iterate<TankSelector>([](auto& tank) { tank.post_update(); });
        mt_iterate<RocketSelector>([](auto& rocket) { rocket.post_update(); });


        // Update BG overlay on main thread since it requires OpenGL calls.
        entities.select<Select<EntityBGOverlay>>().forEach([](auto& entity) { entity.update(); });


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




    void GameController::rebuild_partitions(void) {
        delete bluept;
        delete redpt;

        redpt = new SpatialPartition<EntityTank<Team::RED >>();
        bluept = new SpatialPartition<EntityTank<Team::BLUE>>();


        auto tanks = entities.select<TankSelector>();
        tanks.forEach([&](auto& tank) {
            using type = no_cref<decltype(tank)>;
            getPartitions<type::GetTeam()>().insert(&tank);
            });
    }




    Entities::PolyVector<> GameController::GetInitialEntities(void) {
        Entities::PolyVector<> result;
        result.push_back(EntityBackground());
        result.push_back(EntityBGOverlay());

        constexpr u32   tank_rows = 12;
        constexpr float tank_spacing = 15.0f;
        constexpr Vec2f tank_offset = Vec2f{ 17.0f, WINDOW_HEIGHT - HEALTHBAR_HEIGHT - 18.0f };


        auto place_color = [&](u32 count, auto& dest, const Vec2f& target, bool invert, float side_offset) {
            dest.reserve(count);

            const Vec2f begin = invert ? Vec2f{ WINDOW_WIDTH - tank_offset.x, tank_offset.y } -Vec2f{ side_offset, 0.0f } : tank_offset + Vec2f{ side_offset, 0.0f };
            const Vec2f direction = invert ? Vec2f{ -1.0, -1.0 } : Vec2f{ 1.0, -1.0 };

            u32 r = 0, c = 0;

            for (u32 i = 0; i < count; ++i) {
                dest.push_back({
                    begin + (Vec2f{ r, c } *tank_spacing * direction),
                    target
                    });

                if (++r > tank_rows) {
                    r = 0;
                    ++c;
                }
            }
        };


        place_color(BLUE_TANK_COUNT, result.getT<EntityTank<Team::BLUE>>(), BLUE_TANK_DEFAULT_TARGET, false, 0.0f);
        place_color(RED_TANK_COUNT, result.getT<EntityTank<Team::RED>>(), RED_TANK_DEFAULT_TARGET, true, 135.0f);


        auto& beams = result.getT<EntityBeam>();
        beams.push_back(EntityBeam(BLUE_TANK_DEFAULT_TARGET));
        beams.push_back(EntityBeam(RED_TANK_DEFAULT_TARGET));
        beams.push_back(EntityBeam({ WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0 }));


        auto  text = EntityChar::MakeText({ WINDOW_WIDTH / 2.0 - (5 * EntityChar::KERNING), 100 }, "FRAME: 0000");
        auto& chars = result.getT<EntityChar>();

        std::move(text.begin(), text.end(), std::back_inserter(chars));


        return result;
    }
}