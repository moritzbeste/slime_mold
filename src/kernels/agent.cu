#include <cstdio>
#include "config.h"

__device__ __forceinline__
float2 rotate(float2 v, float angleSin, float angleCos) {
    return make_float2(
        v.x * angleCos - v.y * angleSin,
        v.x * angleSin + v.y * angleCos
    );
}

__device__ __forceinline__
float2 normalize(float2 v) {
    float mag = sqrtf(v.x * v.x + v.y * v.y);

    if (mag < 1e-6f)
        return make_float2(0.f, 0.f);

    return make_float2(v.x / mag, v.y / mag);
}

__device__ __forceinline__
float magnitude2(float3 v) {
    return v.x * v.x +
           v.y * v.y +
           v.z * v.z;
}

__device__ __forceinline__
float sampleSensor(
    float3* buffer,
    float2 position,
    float2 direction,
    float offset,
    int screenWidth,
    int screenHeight) {
    int x = (int)(position.x + direction.x * offset);
    int y = (int)(position.y + direction.y * offset);

    if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) { return 0.0f; }

    float3 data = buffer[y * screenWidth + x];

    return magnitude2(data);
}

__device__ __forceinline__
float rand01(uint seed) {
    seed ^= 2747636419u;
    seed *= 2654435769u;
    seed ^= seed >> 16;
    seed *= 2654435769u;
    seed ^= seed >> 16;
    seed *= 2654435769u;

    return (float)seed / 4294967295.0f;
}

/*
 * The agents kernel updates the position and velocity of an agent and brightens the current position in the surface
 */
__global__ void agent(
    float2* positions,
    float2* velocities,
    float3* writeBuffer,
    float3* readBuffer,
    float3 intensity, 
    float sensorAngleSin,
    float sensorAngleCos,
    float sensorOffset,
    float steeringCoeffSin,
    float steeringCoeffCos,
    float noiseStrength,
    float jitterStrength,
    float deltat,
    int screenWidth,
    int screenHeight,
    int count) {
    uint gid = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (gid >= count) { return; }

    // position update
    float2 position = positions[gid];
    float2 velocity = velocities[gid];
    position.x += velocity.x * deltat;
    position.y += velocity.y * deltat;

    if (position.x < 0) {
        position.x = 0;
        velocity.x *= -1;
    }
    if (position.x >= screenWidth) {
        position.x = screenWidth - 1;
        velocity.x *= -1;
    }
    if (position.y < 0) {
        position.y = 0;
        velocity.y *= -1;
    }
    if (position.y >= screenHeight) {
        position.y = screenHeight - 1;
        velocity.y *= -1;
    }

    // sensor
    float speed = sqrtf(
        velocity.x * velocity.x +
        velocity.y * velocity.y
    );

    float2 dir = normalize(velocity);

    float2 leftDir = rotate(
        dir,
        sensorAngleSin,
        sensorAngleCos
    );

    float2 rightDir = rotate(
        dir,
        -sensorAngleSin,
        sensorAngleCos
    );

    float F  = sampleSensor(readBuffer, position, dir,      sensorOffset, screenWidth, screenHeight);
    float FL = sampleSensor(readBuffer, position, leftDir,  sensorOffset, screenWidth, screenHeight);
    float FR = sampleSensor(readBuffer, position, rightDir, sensorOffset, screenWidth, screenHeight);

    F  += noiseStrength * (rand01(gid * 3 + 0) * 2.f - 1.f);
    FL += noiseStrength * (rand01(gid * 3 + 1) * 2.f - 1.f);
    FR += noiseStrength * (rand01(gid * 3 + 2) * 2.f - 1.f);

    float jitter = jitterStrength * (rand01(gid) * 2.f - 1.f);
    float c = cosf(jitter);
    float s = sinf(jitter);
    dir = rotate(dir, s, c);

    // steer
    if (FL > F && FL > FR) {
        dir = rotate(
            dir,
            steeringCoeffSin,
            steeringCoeffCos
        );
    }
    else if (FR > F && FR > FL) {
        dir = rotate(
            dir,
            -steeringCoeffSin,
            steeringCoeffCos
        );
    }

    velocity.x = dir.x * speed;
    velocity.y = dir.y * speed;

    // drawing
    int x = (int)position.x;
    int y = (int)position.y;

    int idx = y * screenWidth + x;
    float3 data = readBuffer[idx];

    data.x = fminf(data.x + intensity.x, 1.0f);
    data.y = fminf(data.y + intensity.y, 1.0f);
    data.z = fminf(data.z + intensity.z, 1.0f);

    writeBuffer[idx] = data;

    positions[gid] = position;
    velocities[gid] = velocity;
}

void launch_agent(float2* positions, float2* velocities, float3* writeBuffer, float3* readBuffer) {
    agent<<<Config::AGENT_GRID, Config::AGENT_BLOCK>>>(
        positions, 
        velocities, 
        writeBuffer,
        readBuffer, 
        Config::intensity, 
        Config::sensorAngleSin, 
        Config::sensorAngleCos, 
        Config::sensorOffset, 
        Config::steeringCoeffSin,
        Config::steeringCoeffCos,
        Config::noiseStrength,
        Config::jitterStrength,
        Config::delta_t, 
        Config::screenWidth, 
        Config::screenHeight, 
        Config::nAgents);
}