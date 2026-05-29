#include <cstdio>
#include "config.h"

/*
 * The agents kernel updates the position and velocity of an agent and brightens the current position in the surface
 */
__global__ void agent(
    float2* positions,
    float2*velocity,
    cudaGraphicsResource* readRes,
    cudaSurfaceObject_t surface,
    int count
) {
    printf("Hello from CUDA thread: block=%d thread=%d\n",
           blockIdx.x, threadIdx.x);
}

void launch_agent(float2* positions, float2* velocity, cudaGraphicsResource* readRes, cudaSurfaceObject_t surface, int count) {
    agent<<<Config::GRIDSIZE_AGENTS, Config::BLOCKSIZE>>>(positions, velocity, readRes, surface, count);
}