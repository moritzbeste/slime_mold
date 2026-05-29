#include <cstdio>
#include "config.h"

#include <cuda/barrier>
#include <cooperative_groups.h>

/*
 * The render kernel blurs the surface
 */
__global__ void render(
    cudaSurfaceObject_t surface,
    float3 decay,
    int screenWidth,
    int screenHeight, 
    int count) {
    
    uint gid = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (gid >= count) { return; }
    int x = gid % screenWidth;
    int y = gid / screenWidth;
    auto block = cooperative_groups::this_thread_block();

    float4 data = surf2Dread<float4>(surface, x * sizeof(float4), y);
    data.x = fmaxf(data.x - decay.x, 0.f);
    data.y = fmaxf(data.y - decay.y, 0.f);
    data.z = fmaxf(data.z - decay.z, 0.f);
    surf2Dwrite(data, surface, x * sizeof(float4), y);
    block.sync();

    const int BLUR_RADIUS = 2;
    int c = 0;

    for (int i = -BLUR_RADIUS; i <= BLUR_RADIUS; i++) {
        for (int j = -BLUR_RADIUS; j <= BLUR_RADIUS; j++) {
            if (i == 0 && j == 0) { continue; }
            int inRow = x + i;
            int inCol = y + j;

            if (inRow < screenHeight && inRow >= 0 && inCol < screenWidth && inCol >= 0) {
                float4 curr = surf2Dread<float4>(surface, inRow * sizeof(float4), inCol);
                data.x += curr.x;
                data.y += curr.y;
                data.z += curr.z;
                c++;
            }
        }
    }

    data.x /= c;
    data.y /= c;
    data.z /= c;
}

void launch_render(cudaSurfaceObject_t surface) {
    render<<<Config::GRIDSIZE_PIXELS, Config::BLOCKSIZE>>>(surface, Config::decay, Config::screenWidth, Config::screenHeight, Config::n_pixels);
}
