#include <cstdio>
#include "config.h"

/*
 * The render kernel blurs the surface
 */
__global__ void render(
    cudaSurfaceObject_t surface,
    float3* writeBuffer,
    float3 decay,
    int screenWidth,
    int screenHeight) {
    
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= screenWidth || y >= screenHeight) { return; }

    int gid = y * screenWidth + x;

    float3 b = writeBuffer[gid];
    float4 data;
    data.x = b.x; data.y = b.y; data.z = b.z; data.w = 1.f;
    surf2Dwrite(data, surface, x * sizeof(float4), y);

    b.x = fmaxf(b.x - decay.x * b.x * b.x, 0.f);
    b.y = fmaxf(b.y - decay.y * b.y * b.y, 0.f);
    b.z = fmaxf(b.z - decay.z * b.z * b.z, 0.f);
    writeBuffer[gid] = b;
}

void launch_render(cudaSurfaceObject_t surface, float3* writeBuffer) {
    render<<<Config::RENDER_GRID, Config::RENDER_BLOCK>>>(surface, writeBuffer, Config::decay, Config::screenWidth, Config::screenHeight);
}
