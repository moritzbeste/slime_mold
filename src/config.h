#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "raylib.h"

namespace Config {
    inline const int targetFPS = 180;
    inline const int seed = 42; 

    inline constexpr int nAgents = 1 << 20;
    inline constexpr int TILE_SIZE = 1 << 4;
    inline const int AGENT_BLOCK = TILE_SIZE * TILE_SIZE;
    inline const dim3 RENDER_BLOCK = dim3(TILE_SIZE, TILE_SIZE, 1);
    inline const int AGENT_GRID = (nAgents + AGENT_BLOCK - 1) / AGENT_BLOCK;

    inline int screenWidth;
    inline int screenHeight;
    inline int nPixels;
    inline float2 center;
    inline dim3 RENDER_GRID;

    inline void Init() {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        nPixels = screenWidth * screenHeight;
        center = float2(screenWidth / 2.f, screenHeight / 2.f);
        RENDER_GRID = dim3((screenWidth + TILE_SIZE - 1) / TILE_SIZE, (screenHeight + TILE_SIZE - 1) / TILE_SIZE, 1);
    }

    inline const float delta_t = 2.f;
    inline const float3 intensity = float3(0.15f, 0.1f, 0.025f);
    inline const float3 decay = float3(0.2f, 0.1f, 0.2f);

    inline const float sensorAngle =  M_PI / 6;
    inline const float sensorAngleSin = sin(sensorAngle); // sin(-x) = -sin(x)
    inline const float sensorAngleCos = cos(sensorAngle); // cos(-X) = cos(x)
    inline const float sensorOffset = 8.f;
    inline const float steeringCoeff = 0.7f;
    inline const float steeringCoeffSin = sin(steeringCoeff); // sin(-x) = -sin(x)
    inline const float steeringCoeffCos = cos(steeringCoeff); // cos(-X) = cos(x)
    inline const float noiseStrength = 5e-3;
    inline const float jitterStrength = 5e-3;
}
