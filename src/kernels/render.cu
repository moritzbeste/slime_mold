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
    int blurRadius,
    int count) {
    
    uint gid = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (gid >= count) { return; }
    int x = gid % screenWidth;
    int y = gid / screenWidth;
    auto block = cooperative_groups::this_thread_block();

    float4 data = surf2Dread<float4>(surface, x * sizeof(float4), y);
    data.x = fmaxf(data.x * decay.x, 0.f);
    data.y = fmaxf(data.y * decay.y, 0.f);
    data.z = fmaxf(data.z * decay.z, 0.f);
    surf2Dwrite(data, surface, x * sizeof(float4), y);
    block.sync();

    int c = 0;

    for (int i = -blurRadius; i <= blurRadius; i++) {
        for (int j = -blurRadius; j <= blurRadius; j++) {
            if (i == 0 && j == 0) { continue; }
            int curr_x = x + i;
            int curr_y = y + j;

            if (curr_x < screenHeight && curr_x >= 0 && curr_y < screenWidth && curr_y >= 0) {
                float4 curr = surf2Dread<float4>(surface, curr_x * sizeof(float4), curr_y);
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
    render<<<Config::GRIDSIZE_PIXELS, Config::BLOCKSIZE>>>(surface, Config::decay, Config::screenWidth, Config::screenHeight, Config::blurRadius, Config::nPixels);
}
