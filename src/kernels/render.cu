#include <cstdio>
#include "config.h"

/*
 * The render kernel blurs the surface
 */
__global__ void render(
    cudaSurfaceObject_t surface,
    float3 decay,
    int screenWidth,
    int count
) {
    uint gid = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (gid >= count) { return; }

    int x = gid % screenWidth;
    int y = gid / screenWidth;

    float4 data = surf2Dread<float4>(surface, x * sizeof(float4), y);
    data.x = fmaxf(data.x - decay.x, 0.f);
    data.y = fmaxf(data.y - decay.y, 0.f);
    data.z = fmaxf(data.z - decay.z, 0.f);
    surf2Dwrite(data, surface, x * sizeof(float4), y);
}

void launch_render(cudaSurfaceObject_t surface) {
    render<<<Config::GRIDSIZE_PIXELS, Config::BLOCKSIZE>>>(surface, Config::decay, Config::screenWidth, Config::n_pixels);
}