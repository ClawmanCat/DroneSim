#pragma once

#include "Core.h"

#include <vector>

namespace DroneSim::Game {
    template <typename T> class SpatialPartition {
    public:
        using Node = std::vector<T*>;

        // THe size of the area that is partitioned.
        constexpr static u32 AREA_EXP     = 12;
        constexpr static u32 AREA         = 1u << AREA_EXP;
        // The size of one partition.
        constexpr static u32 CHUNK_EXP    = 5;
        constexpr static u32 CHUNK        = 1u << CHUNK_EXP;
        // Half the size of the area. The area is contained in [-HF_AREA, HF_AREA)
        constexpr static u32 HF_AREA_EXP  = AREA_EXP - 1;
        constexpr static u32 HF_AREA      = 1u << HF_AREA_EXP;
        // The side length of the area, expressed in partitions.
        constexpr static u32 AREA_LEN_EXP = AREA_EXP - CHUNK_EXP;
        constexpr static u32 AREA_LEN     = 1u << AREA_LEN_EXP;
        // The amount of partitions in the area.
        constexpr static u32 NUM_CHUNKS   = AREA_LEN * AREA_LEN;


        SpatialPartition(void) : min({ 0, 0 }), max({ 0, 0 }) {
            std::fill(partitions.begin(), partitions.end(), std::vector<T*>());
        }


        void insert(T* elem) {
            const Vec2ui index = Vec2ui(elem->getPosition() + (float) HF_AREA) >> CHUNK_EXP;
            
            if (index.x < min.x) min.x = index.x;
            if (index.y < min.y) min.y = index.y;

            if (index.x > max.x) max.x = index.x;
            if (index.y > max.y) max.y = index.y;


            partitions[flatten_index(index)].push_back(elem);
        }


        T* closest(const Vec2f& where) {
            return closest(where, [](auto* elem) { return true; });
        }


        template <typename Pred> T* closest(const Vec2f& where, const Pred& pred) {
            const static Vec2i DIRECTIONS[] = {
                { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 }, { 0, 0 }
            };


            Vec2ui center_chunk = Vec2ui(where + (float) HF_AREA) >> CHUNK_EXP;


            T* closest_elem = nullptr;
            float closest_dsq = std::numeric_limits<float>::infinity();
            u32 radius = 0;


            // Increase the radius if it doesn't overlap the area with objects in it.
            if (glm::clamp(center_chunk, min, max) != center_chunk) {
                Vec2i delta = glm::clamp(center_chunk, min, max) - center_chunk;
                radius = std::min(std::abs(delta.x), std::abs(delta.y));
            }


            while (!closest_elem) {
                const Vec2ui start = center_chunk + radius;
                Vec2ui next = start;
                u32 direction = 0;

                do {
                    // Check current node.
                    if (glm::clamp(next, min, max) == next) {
                        for (T* elem : partitions[flatten_index(next)]) {
                           const float dsq = distance_squared(where, elem->getPosition());

                           if (dsq < closest_dsq && pred(elem)) {
                               closest_elem = elem;
                               closest_dsq = dsq;
                           }
                        }   
                    }


                    // Move in a square of size 'radius' around the center.
                    // If moving along the current direction vector would move us outside of the square, turn.
                    const Vec2i n = Vec2i(next) + DIRECTIONS[direction];
                    
                    if (
                        n.x < center_chunk.x - radius || n.x > center_chunk.x + radius ||
                        n.y < center_chunk.y - radius || n.y > center_chunk.y + radius
                    ) ++direction;


                    next += DIRECTIONS[direction];
                } while (!(radius == 0 || next == start));

                ++radius;
            }
            
            return closest_elem;
        }


        // We can't take the *actual* distance and just square it, because the original simulation takes
        // 2(R^2) as the distance squared instead of (2R)^2, and their collision detection logic breaks with the correct distance.
        std::vector<T*> nearby(const Vec2f& where, float fake_distance_sq, float distance) {
            std::vector<T*> result;

            const Vec2ui center_chunk = Vec2ui(where + (float) HF_AREA) >> CHUNK_EXP;
            const u32 farthest_chunk  = (((u32) std::ceil(distance)) >> CHUNK_EXP) + 1;
            const float dsq           = fake_distance_sq;

            const Vec2ui areaMin = glm::clamp(center_chunk - farthest_chunk, min, max);
            const Vec2ui areaMax = glm::clamp(center_chunk + farthest_chunk, min, max);

            for (u32 x = areaMin.x; x <= areaMax.x; ++x) {
                for (u32 y = areaMin.y; y <= areaMax.y; ++y) {
                    //if (distance_to_partition_squared(where, { x, y }) > dsq) continue;

                    for (T* elem : partitions[flatten_index({ x, y })]) {
                        if (distance_squared(elem->getPosition(), where) <= dsq) result.push_back(elem);
                    }
                }
            }

            return result;
        }
    private:
        std::array<Node, NUM_CHUNKS> partitions;
        // Since we rebuild our partitions every frame anyway,
        // we can keep easily keep track of the area of the map that actually has anything in it.
        Vec2ui min, max;


        // Converts a world position to a chunk index.
        constexpr static u32 flatten_position(const Vec2f& v) {
            return flatten_index(Vec2ui(v + (float) HF_AREA) >> CHUNK_EXP);
        }


        // Converts a chunk position to a chunk index.
        constexpr static u32 flatten_index(const Vec2ui& v) {
            return v.x + (v.y << AREA_LEN_EXP);
        }


        // Converts a chunk index to a chunk position.
        constexpr static Vec2ui unflatten_index(u32 i) {
            u32 y = i >> CHUNK_EXP;
            return { i - (y << CHUNK_EXP), y };
        }


        constexpr static float distance_squared(const Vec2f& a, const Vec2f& b) {
            const Vec2f delta = (b - a);
            return glm::dot(delta, delta);
        }


        constexpr static float distance_to_partition_squared(const Vec2f& point, const Vec2ui& partition) {
            const Vec2f clamped = (Vec2f) glm::clamp(Vec2ui(point), partition << CHUNK_EXP, (partition + 1u) << CHUNK_EXP);
            return distance_squared(point + (float) HF_AREA, clamped);
        }
    };
}