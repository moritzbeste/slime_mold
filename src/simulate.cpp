#include "raylib.h"
#include "rlgl.h"
#include "utility.h"
#include "config.h"
#include "kernels/agent.cuh"
#include "kernels/render.cuh"

#include <iostream>
#include <glad/gl.h>
#include <vector>

int simulate(void) {

    initRandom();
    InitWindow(0, 0, "Slime Mold Simulation");
    SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
    Config::Init();
    SetTargetFPS(Config::targetFPS);
    gladLoadGL((GLADloadfunc)rlGetProcAddress);
    cudaSetDevice(0);

    // generate texture
    GLuint tex = generateOpenGLTexture(Config::screenWidth, Config::screenHeight);
    Texture2D rlTex = generateRaylibTexture(Config::screenWidth, Config::screenHeight, tex);
    cudaGraphicsResource* cudaResource = registerWithCuda(tex);
    cudaSurfaceObject_t surface;

    // generate texture buffers
    float3* writeBuffer = genTexture(Config::screenWidth * Config::screenHeight);
    float3* readBuffer = genTexture(Config::screenWidth * Config::screenHeight);

    // generate agents
    float2* d_positions = cudaBuffer(Config::nAgents, false);
    float2* d_velocities = cudaBuffer(Config::nAgents, true);

    while (!WindowShouldClose()) {
        CHECK(cudaGraphicsMapResources(1, &cudaResource));
        surface = getCudaSurface(cudaResource);

        launch_agent(d_positions, d_velocities, writeBuffer, readBuffer);
        CHECK(cudaDeviceSynchronize());
        launch_render(surface, writeBuffer);
        CHECK(cudaDeviceSynchronize());

        CHECK(cudaDestroySurfaceObject(surface));
        CHECK(cudaGraphicsUnmapResources(1, &cudaResource));

        BeginDrawing();
        DrawTexture(rlTex, 0, 0, WHITE);
        EndDrawing();

        CHECK(cudaMemcpy(
            readBuffer,
            writeBuffer,
            Config::nPixels * sizeof(float3),
            cudaMemcpyDeviceToDevice
        ));
    }

    CloseWindow();

    CHECK(cudaFree(d_positions));
    CHECK(cudaFree(d_velocities));
    CHECK(cudaFree(writeBuffer));
    CHECK(cudaFree(readBuffer));

    return 0;
}
