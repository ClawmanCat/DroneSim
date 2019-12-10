#pragma once

#include "Core.h"

#include <vector>

namespace DroneSim::Game {
    class EntityTank;
    class EntityRocket;

    class ParticleSmoke;
    class ParticleExplosion;
    class ParticleBeam;


    class GameController {
    public:
    private:
        std::vector<EntityTank>         tanks;
        std::vector<EntityRocket>       rockets;

        std::vector<ParticleSmoke>      smoke;
        std::vector<ParticleExplosion>  explosions;
        std::vector<ParticleBeam>       beams;

        u64 frames = 0;
    };
}