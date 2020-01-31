#pragma once

#include "Core.h"
#include "WindowController.h"
#include "EntityList.h"
#include "GameConstants.h"
#include "Renderer2D.h"
#include "Timer.h"
#include "PolyVector.h"
#include "EntitySelectors.h"
#include "SpatialPartition.h"

#include <vector>
#include <string>
#include <algorithm>
#include <future>
#include <mutex>


namespace DroneSim::Game {
    class GameController {
    public:
        static GameController& instance(void);
        static std::mutex& getEntitiesMtx(void);


        ~GameController(void);


        void start(std::vector<std::string>&& args);


        auto& getEntities(void) { return entities; }
        const auto& getEntities(void) const { return entities; }
        
        template <Team team> auto& getPartitions(void) { if constexpr (team == Team::BLUE) return *bluept; else return *redpt; }
        template <Team team> const auto& getPartitions(void) const { if constexpr (team == Team::BLUE) return *bluept; else return *redpt; }

        u32 getFrameCount(void) const { return frames; }
    private:
        constexpr static milliseconds reference_time = 73'520ms;
        Utility::Timer timer;

        std::vector<std::string> args;
        u64 frames = 0;

        Entities::PolyVector<> entities;

        // Tank position partitioned storage.
        SpatialPartition<EntityTank<Team::BLUE>>* bluept;
        SpatialPartition<EntityTank<Team::RED >>* redpt;

        std::array<EntityChar*, 4> counter;     // Frame counter.

        Render::Renderer2D renderer;




        GameController(void);


        void tick(void);
        void loop(void);

        void rebuild_partitions(void);

        static Entities::PolyVector<> GetInitialEntities(void);


        template <typename Selector, typename Pred> void mt_iterate(const Pred& pred) {
            std::vector<std::future<void>> handles;
            const u32 num_workers = 2 * std::thread::hardware_concurrency();

            entities.select<Selector>().forEachSub([&](auto& v) {
                u32 batch_size = (v.size() / num_workers) + 1;

                for (u32 i = 0; i < std::min(num_workers, (u32)v.size()); ++i) {
                    auto h = std::async(std::launch::async, [i, batch_size, pred, &v]() {
                        const u32 size = v.size();

                        for (u32 j = batch_size * i; j < std::min(batch_size * (i + 1), size); ++j) pred(v[j]);
                        });

                    handles.push_back(std::move(h));
                }
                });


            for (auto& handle : handles) handle.get();
        }
    };
}