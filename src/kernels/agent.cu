#include <cstdio>
#include "config.h"

/*
 * The agents kernel updates the position and velocity of an agent and brightens the current position in the surface
 */
__global__ void agent(
    float2* positions,
    float2* velocities,
    cudaSurfaceObject_t surface,
    float3 intensity, 
    float deltat,
    int screenWidth,
    int screenHeight,
    int count
) {
    uint gid = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (gid >= count) { return; }

    float2 position = positions[gid];
    float2 velocity = velocities[gid];
    position.x += velocity.x * deltat;
    position.y += velocity.y * deltat;

    if (position.x < 0) {
        position.x = 0;
        velocity.x *= -1;
    }
    if (position.x >= screenWidth) {
        position.x = screenWidth - 1;
        velocity.x *= -1;
    }
    if (position.y < 0) {
        position.y = 0;
        velocity.y *= -1;
    }
    if (position.y >= screenHeight) {
        position.y = screenHeight - 1;
        velocity.y *= -1;
    }

    int x = (int) position.x;
    int y = (int) position.y;
    float4 data = surf2Dread<float4>(surface, x * sizeof(float4), y);

    data.x = fminf(data.x + intensity.x, 1.0f);
    data.y = fminf(data.y + intensity.y, 1.0f);
    data.z = fminf(data.z + intensity.z, 1.0f);
    data.w = 1.f;

    surf2Dwrite(data, surface, x * sizeof(float4), y);

    positions[gid] = position;
    velocities[gid] = velocity;
}

void launch_agent(float2* positions, float2* velocities, cudaSurfaceObject_t surface) {
    agent<<<Config::GRIDSIZE_AGENTS, Config::BLOCKSIZE>>>(positions, velocities, surface, Config::intensity, Config::deltat, Config::screenWidth, Config::screenHeight, Config::n_agents);
}