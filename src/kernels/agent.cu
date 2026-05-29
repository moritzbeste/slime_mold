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
float magnitude2(float4 v) {
    return v.x * v.x +
           v.y * v.y +
           v.z * v.z;
}

__device__ __forceinline__
float sampleSensor(
    cudaSurfaceObject_t surface,
    float2 position,
    float2 direction,
    float offset,
    int screenWidth,
    int screenHeight) {
    int x = (int)(position.x + direction.x * offset);
    int y = (int)(position.y + direction.y * offset);

    if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) { return 0.0f; }

    float4 data = surf2Dread<float4>(
        surface,
        x * sizeof(float4),
        y
    );

    return magnitude2(data);
}

/*
 * The agents kernel updates the position and velocity of an agent and brightens the current position in the surface
 */
__global__ void agent(
    float2* positions,
    float2* velocities,
    cudaSurfaceObject_t surface,
    float3 intensity, 
    float sensor_angle_sin,
    float sensor_angle_cos,
    float sensor_offset,
    float steering_coeff_sin,
    float steering_coeff_cos,
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
        sensor_angle_sin,
        sensor_angle_cos
    );

    float2 rightDir = rotate(
        dir,
        -sensor_angle_sin,
        sensor_angle_cos
    );

    float F  = sampleSensor(surface, position, dir,      sensor_offset, screenWidth, screenHeight);
    float FL = sampleSensor(surface, position, leftDir,  sensor_offset, screenWidth, screenHeight);
    float FR = sampleSensor(surface, position, rightDir, sensor_offset, screenWidth, screenHeight);

    // steer
    if (FL > F && FL > FR) {
        dir = rotate(
            dir,
            steering_coeff_sin,
            steering_coeff_cos
        );
    }
    else if (FR > F && FR > FL) {
        dir = rotate(
            dir,
            -steering_coeff_sin,
            steering_coeff_cos
        );
    }

    velocity.x = dir.x * speed;
    velocity.y = dir.y * speed;

    // drawing
    int x = (int) position.x;
    int y = (int) position.y;
    float4 data = surf2Dread<float4>(surface, x * sizeof(float4), y);

    data.x = fminf(data.x + intensity.x, 1.0f);
    data.y = fminf(data.y + intensity.y, 1.0f);
    data.z = fminf(data.z + intensity.z, 1.0f);
    data.w = 1.f;

    surf2Dwrite(data, surface, x * sizeof(float4), y);

    positions[gid] = position;
    velocities[gid] = velocity;
}

void launch_agent(float2* positions, float2* velocities, cudaSurfaceObject_t surface) {
    agent<<<Config::GRIDSIZE_AGENTS, Config::BLOCKSIZE>>>(positions, 
        velocities, 
        surface, 
        Config::intensity, 
        Config::sensor_angle_sin, 
        Config::sensor_angle_cos, 
        Config::sensor_offset, 
        Config::steering_coeff_sin,
        Config::steering_coeff_cos,
        Config::deltat, 
        Config::screenWidth, 
        Config::screenHeight, 
        Config::n_agents);
}