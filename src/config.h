#pragma once
#include <cmath>

namespace Config {
    inline constexpr int targetFPS = 60;
    inline constexpr int seed = 42; 
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int n_pixels = screenWidth * screenHeight;

    const int n_agents = (int)pow(2, 14);
    const int BLOCKSIZE = (int)pow(2, 8);
    const int GRIDSIZE_AGENTS = n_agents / BLOCKSIZE;
    const int GRIDSIZE_PIXELS = (n_pixels + BLOCKSIZE - 1) / BLOCKSIZE;

    const float magnitude = 50.0;
}