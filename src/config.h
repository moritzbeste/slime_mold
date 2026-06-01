#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "raylib.h"

namespace Config {
    inline const int targetFPS = 180;
    inline const int seed = 42; 

    inline constexpr int nAgents = 1 << 22;
    inline constexpr int BLOCKSIZE = 1 << 9;
    inline const int GRIDSIZE_AGENTS = (nAgents + BLOCKSIZE - 1) / BLOCKSIZE;

    inline int screenWidth;
    inline int screenHeight;
    inline int nPixels;
    inline int GRIDSIZE_PIXELS;

    inline void Init() {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        nPixels = screenWidth * screenHeight;
        GRIDSIZE_PIXELS = (nPixels + BLOCKSIZE - 1) / BLOCKSIZE;
    }

    inline const float delta_t = 1.f;
    inline const float3 intensity = float3(0.06f, 0.06f, 0.06f);
    inline const float3 decay = float3(0.08f, 0.3f, 0.03f);

    inline const float sensorAngle =  M_PI / 8;
    inline const float sensorAngleSin = sin(sensorAngle); // sin(-x) = -sin(x)
    inline const float sensorAngleCos = cos(sensorAngle); // cos(-X) = cos(x)
    inline const float sensorOffset = 10.f;
    inline const float steeringCoeff = 0.1f;
    inline const float steeringCoeffSin = sin(steeringCoeff); // sin(-x) = -sin(x)
    inline const float steeringCoeffCos = cos(steeringCoeff); // cos(-X) = cos(x)
    inline const float noiseStrength = 1e-2;
    inline const float jitterStrength = 0;
}
