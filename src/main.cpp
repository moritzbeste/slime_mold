#include "raylib.h"
#include "utility.h"
#include "config.h"
#include <iostream>

int main(void)
{
    const int seed = 42;
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitRandom();

    InitWindow(screenWidth, screenHeight, "raylib [core] example - delta time");
    SetTargetFPS(Config::targetFPS);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("RandomFloat()", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}