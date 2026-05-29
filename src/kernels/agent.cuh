#pragma once

void launch_agent(float2* positions, float2* velocity, cudaGraphicsResource* readRes, cudaSurfaceObject_t surface, int count);