#include <cstdio>
#include "config.h"

/*
 * The render kernel blurs the surface
 */
__global__ void render(
    cudaSurfaceObject_t surface,
    float3* buffer,
    float3 decay,
    int screenWidth,
    int count) {
    
    uint gid = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (gid >= count) { return; }
    int x = gid % screenWidth;
    int y = gid / screenWidth;
    
    float3 b = buffer[gid];
    float4 data;
    data.x = b.x; data.y = b.y; data.z = b.z; data.w = 1.f;
    surf2Dwrite(data, surface, x * sizeof(float4), y);

    b.x = fmaxf(b.x - decay.x * b.x * b.x, 0.f);
    b.y = fmaxf(b.y - decay.y * b.y * b.y, 0.f);
    b.z = fmaxf(b.z - decay.z * b.z * b.z, 0.f);
    buffer[gid] = b;
}

void launch_render(cudaSurfaceObject_t surface, float3* buffer) {
    render<<<Config::GRIDSIZE_PIXELS, Config::BLOCKSIZE>>>(surface, buffer, Config::decay, Config::screenWidth, Config::nPixels);
}
