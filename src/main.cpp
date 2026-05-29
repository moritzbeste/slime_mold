#include "raylib.h"
#include "rlgl.h"
#include "utility.h"
#include "config.h"
#include "kernels/agent.cuh"
#include "kernels/render.cuh"

#include <iostream>
#include <glad/gl.h>
#include <vector>

int main(void) {

    initRandom();
    InitWindow(Config::screenWidth, Config::screenHeight, "opengl test");
    SetTargetFPS(Config::targetFPS);
    gladLoadGL((GLADloadfunc)rlGetProcAddress);

    // generate texture
    GLuint tex = generateOpenGLTexture(Config::screenWidth, Config::screenHeight);
    Texture2D rlTex = generateRaylibTexture(Config::screenWidth, Config::screenHeight, tex);
    cudaGraphicsResource* cudaResource = registerWithCuda(tex);

    // generate agents (SoA structure)
    float2* d_positions = cudaBuffer(Config::n_agents, 0.0);
    float2* d_velocities = cudaBuffer(Config::n_agents, Config::magnitude);

    while (!WindowShouldClose()) {
        CHECK(cudaGraphicsMapResources(1, &cudaResource));
        cudaSurfaceObject_t surface = getCudaSurface(cudaResource);

        launch_agent(d_positions, d_velocities, cudaResource, surface, Config::n_agents);
        launch_render(surface, Config::n_pixels);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(rlTex, 0, 0, WHITE);
        EndDrawing();

        CHECK(cudaDestroySurfaceObject(surface));
        CHECK(cudaGraphicsUnmapResources(1, &cudaResource));
        CHECK(cudaDeviceSynchronize());
    }

    CloseWindow();

    CHECK(cudaFree(d_positions));
    CHECK(cudaFree(d_velocities));

    return 0;
}
