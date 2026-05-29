#pragma once
#include <cmath>

namespace Config {
    inline constexpr int targetFPS = 60;
    inline constexpr int seed = 42; 
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int n_pixels = screenWidth * screenHeight;

    const int n_agents = (int)pow(2, 5);
    const int BLOCKSIZE = (int)pow(2, 8);
    const int GRIDSIZE_AGENTS = (n_agents + BLOCKSIZE - 1) / BLOCKSIZE;
    const int GRIDSIZE_PIXELS = (n_pixels + BLOCKSIZE - 1) / BLOCKSIZE;

    const float magnitude = 50.0;
    const float deltat = 1e-2;
    const float3 intensity = float3(0.2f, 0.1f, 0.1f);
    const float3 decay = float3(0.002f, 0.001f, 0.001f);
}