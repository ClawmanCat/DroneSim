#pragma once

#include "Core.h"

#include <string_view>

namespace DroneSim::Game {
    constexpr static std::string_view WINDOW_TITLE      = "Drone Simulator";
    constexpr static u32 WINDOW_WIDTH                   = 1280;
    constexpr static u32 WINDOW_HEIGHT                  = 720;
    constexpr static u32 SIMULATED_FRAMES               = 2000;
    constexpr static u32 HEALTHBAR_HEIGHT               = 71;


    constexpr static u32 BLUE_TANK_COUNT                = 1279;
    constexpr static u32 RED_TANK_COUNT                 = 1279;

    constexpr static Vec2f BLUE_TANK_DEFAULT_TARGET     = Vec2f{ 1252.0f, WINDOW_HEIGHT - 626.0f };
    constexpr static Vec2f RED_TANK_DEFAULT_TARGET      = Vec2f{ 131.0f,  WINDOW_HEIGHT - HEALTHBAR_HEIGHT - 35.0f  };

    constexpr static u32 TANK_MAX_HEALTH                = 1000;
    constexpr static u32 ROCKET_DAMAGE                  = 60;
    constexpr static u32 BEAM_DAMAGE                    = 50;

    constexpr static float TANK_MAX_SPEED               = 1.5;      // Misleading name, see EntityTank.cpp
    constexpr static float ROCKET_MAX_SPEED             = 3.0;
}