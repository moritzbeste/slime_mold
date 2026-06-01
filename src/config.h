#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "raylib.h"

namespace Config {
    inline const int targetFPS = 180;
    inline const int seed = 42; 

    inline constexpr int nAgents = 1 << 20;
    inline constexpr int BLOCKSIZE = 1 << 8;
    inline const int GRIDSIZE_AGENTS = (nAgents + BLOCKSIZE - 1) / BLOCKSIZE;

    inline int screenWidth;
    inline int screenHeight;
    inline int nPixels;
    inline int GRIDSIZE_PIXELS;
    inline float2 center;

    inline void Init() {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        nPixels = screenWidth * screenHeight;
        center = float2(screenWidth / 2.f, screenHeight / 2.f);
        GRIDSIZE_PIXELS = (nPixels + BLOCKSIZE - 1) / BLOCKSIZE;
    }

    inline const float delta_t = 2.f;
    inline const float3 intensity = float3(0.5f, 0.4f, 0.1f);
    inline const float3 decay = float3(0.3f, 0.15f, 0.3f);

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
