#include "raylib.h"
#include "rlgl.h"
#include "utility.h"
#include "config.h"
#include "kernels/kernel.cuh"
#include <iostream>
#include <glad/gl.h>
#include <vector>

int main(void) {

    initRandom();

    launch_hello_kernel();

    InitWindow(Config::screenWidth, Config::screenHeight, "opengl test");
    SetTargetFPS(Config::targetFPS);
    gladLoadGL((GLADloadfunc)rlGetProcAddress);

    // generate texture
    GLuint tex = generateOpenGLTexture(Config::screenWidth, Config::screenHeight);
    Texture2D rlTex = generateRaylibTexture(Config::screenWidth, Config::screenHeight, tex);
    cudaGraphicsResource* cudaResource = registerWithCuda(tex);
    cudaArray_t cudaArray = getCudaArray(cudaResource);

    // generate agents (SoA structure)
    float2* d_positions = cudaBuffer(Config::n_agents, 0.0);
    float2* d_velocities = cudaBuffer(Config::n_agents, Config::magnitude);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(rlTex, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}