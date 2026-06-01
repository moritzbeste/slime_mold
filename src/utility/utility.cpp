#include "utility.h"
#include "config.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>

void initRandom() {
    srand(Config::seed);
}

float randomFloat() {
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return r;
}

float2 randomFloat2(float magnitude) {
    float theta = randomFloat() * 2.0f * PI;
    return float2(cos(theta) * magnitude, sin(theta) * magnitude);
}

GLuint generateOpenGLTexture(int width, int height) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int len = width * height * 4;
    std::vector<float> data(len, 0.f);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                width, height, 0,
                GL_RGBA, GL_FLOAT, data.data());

    return tex;
}

Texture2D generateRaylibTexture(int width, int height, GLuint id) {
    Texture2D rlTex;
    rlTex.id = id;
    rlTex.width = width;
    rlTex.height = height;
    rlTex.mipmaps = 1;
    rlTex.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
    return rlTex;
}

cudaGraphicsResource* registerWithCuda(GLuint tex) {
    cudaGraphicsResource* cudaResource;
    CHECK(cudaGraphicsGLRegisterImage(
        &cudaResource,
        tex,
        GL_TEXTURE_2D,
        cudaGraphicsRegisterFlagsSurfaceLoadStore
    ));
    
    return cudaResource;
}

cudaSurfaceObject_t getCudaSurface(cudaGraphicsResource* cudaResource) {
    cudaArray_t array;
    CHECK(cudaGraphicsSubResourceGetMappedArray(&array, cudaResource, 0, 0));

    cudaResourceDesc desc{};
    desc.resType = cudaResourceTypeArray;
    desc.res.array.array = array;

    cudaSurfaceObject_t surface;
    CHECK(cudaCreateSurfaceObject(&surface, &desc));

    return surface;
}


float2* cudaBuffer(int length, bool magnitude) {
    float2* d_buffer;
    CHECK(cudaMalloc(&d_buffer, length * sizeof(float2)));
    std::vector<float2> host(length);

    for (int i = 0; i < length; i++) {
        float r = randomFloat();
        host[i] = magnitude ? randomFloat2(exp(r *r) / sqrt(M_E)) : float2(Config::screenWidth / 2, Config::screenHeight / 2);
    }

    CHECK(cudaMemcpy(d_buffer, host.data(), length * sizeof(float2), cudaMemcpyHostToDevice));

    return d_buffer;
}

float3* genTexture(int length) {
    float3* d_buffer;
    CHECK(cudaMalloc(&d_buffer, length * sizeof(float3)));
    std::vector<float> host(length * 3, 0.f);

    CHECK(cudaMemcpy(d_buffer, host.data(), length * sizeof(float3), cudaMemcpyHostToDevice));

    return d_buffer;
}