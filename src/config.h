#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace Config {
    inline constexpr int targetFPS = 60;
    inline constexpr int seed = 42; 
    
    const int screenWidth = 1080;
    const int screenHeight = 720;
    const int n_pixels = screenWidth * screenHeight;

    const int n_agents = (int)pow(2, 14);
    const int BLOCKSIZE = (int)pow(2, 8);
    const int GRIDSIZE_AGENTS = (n_agents + BLOCKSIZE - 1) / BLOCKSIZE;
    const int GRIDSIZE_PIXELS = (n_pixels + BLOCKSIZE - 1) / BLOCKSIZE;

    const float magnitude = 100.f;
    const float deltat = 1e-2;
    const float3 intensity = float3(0.2f, 0.1f, 0.2f);
    const float3 decay = float3(0.004f, 0.003f, 0.002f);

    const float sensor_angle = 5 * M_PI / 12;
    const float sensor_angle_sin = sin(sensor_angle); // sin(-x) = -sin(x)
    const float sensor_angle_cos = cos(sensor_angle); // cos(-X) = cos(x)
    const float sensor_offset = 5.f;
    const float steering_coeff = 0.05f;
    const float steering_coeff_sin = sin(steering_coeff); // sin(-x) = -sin(x)
    const float steering_coeff_cos = cos(steering_coeff); // cos(-X) = cos(x)
}