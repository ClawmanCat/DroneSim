#pragma once

#include "Core.h"

#include <string_view>

namespace DroneSim::Game {
    constexpr static u32 BLUE_TANK_COUNT                = 1279;
    constexpr static u32 RED_TANK_COUNT                 = 1279;

    constexpr static u32 TANK_MAX_HEALTH                = 1000;
    constexpr static u32 ROCKET_DAMAGE                  = 60;
    constexpr static u32 BEAM_DAMAGE                    = 50;

    constexpr static float TANK_SPEED                   = 1.5;

    constexpr static u32 SIMULATED_FRAMES               = 2000;
    constexpr static u32 HEALTHBAR_SIZE_X               = 70;
    constexpr static u32 HEALTHBAR_SIZE_Y               = 1;

    constexpr static std::string_view WINDOW_TITLE      = "Drone Simulator";
    constexpr static u32 WINDOW_WIDTH                   = 1280;
    constexpr static u32 WINDOW_HEIGHT                  = 720;
}