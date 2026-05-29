#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace Config {
    inline constexpr int targetFPS = 60;
    inline constexpr int seed = 42; 
    
    const int screenWidth = 1500;
    const int screenHeight = 1000;
    const int nPixels = screenWidth * screenHeight;

    const int nAgents = (int)pow(2, 15);
    const int BLOCKSIZE = (int)pow(2, 8);
    const int GRIDSIZE_AGENTS = (nAgents + BLOCKSIZE - 1) / BLOCKSIZE;
    const int GRIDSIZE_PIXELS = (nPixels + BLOCKSIZE - 1) / BLOCKSIZE;

    const float magnitude = 100.f;
    const float delta_t = 1e-2;
    const float3 intensity = float3(0.5f, 0.2f, 0.2f);
    const float3 decay = float3(0.7f, 0.99f, 0.999f);

    const float sensorAngle = 5 * M_PI / 12;
    const float sensorAngleSin = sin(sensorAngle); // sin(-x) = -sin(x)
    const float sensorAngleCos = cos(sensorAngle); // cos(-X) = cos(x)
    const float sensorOffset = 5.f;
    const float steeringCoeff = 0.07f;
    const float noiseStrength = 1e-10;
    const float jitterStrength = 1e-8;
    const float steeringCoeffSin = sin(steeringCoeff); // sin(-x) = -sin(x)
    const float steeringCoeffCos = cos(steeringCoeff); // cos(-X) = cos(x)
    const int blurRadius = 2;
}
