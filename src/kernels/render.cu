#include <cstdio>
#include "config.h"

/*
 * The render kernel blurs the surface
 */
__global__ void render(
    cudaSurfaceObject_t surface,
    int count
) {

}

void launch_render(cudaSurfaceObject_t surface, int count) {
    render<<<Config::GRIDSIZE_PIXELS, Config::BLOCKSIZE>>>(surface, count);
}