#pragma once
#include <glad/gl.h>
#include <vector>
#include "raylib.h"
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#define CHECK(X)                                                    \
    do {                                                            \
        cudaError_t err = (X);                                      \
        if (err != cudaSuccess) {                                   \
            fprintf(stderr,                                         \
                "CUDA Runtime error at %s:%d (%s): %s\n",           \
                __FILE__, __LINE__, #X, cudaGetErrorString(err));   \
        }                                                           \
    } while (0)

/*
 *   initiate random generation using the seed given in config
 */
void initRandom();

/*
 *   generate random float in [0, 1]
 */
float randomFloat();

/*
 *   generate random float2 in [0, magnitude]^2
 *   params: float magnitude gives the target magnitude of the vector
 */
float2 randomFloat2(float magnitude);

/*
 *   generate OpenGL texture with a given width and height
 *   params: int width, int height set the dimensions of the texture
 */
GLuint generateOpenGLTexture(int width, int height);

/*
 *   generate raylib texture with a given width and height and fill it with an OpenGL texture
 *   params: int width, int height set the dimensions of the texture 
 *   note: caller is responsible for updating OpenGL texture id
 */
Texture2D generateRaylibTexture(int width, int height, GLuint id);

/*
 *   register OpenGL texture with CUDA
 *   params: GLuint tex to be registered
 *   returns: cudaGraphicsResource reference
 */
cudaGraphicsResource* registerWithCuda(GLuint tex);

/*
 *   generate cudaSurfaceObject_t from cudaGraphicsResource
 */
cudaSurfaceObject_t getCudaSurface(cudaGraphicsResource* cudaResource);

/*
 *   allocate a cuda buffer and fill it using float2 vectors given a magnitude
 *   params: int length is the length of the buffer
 *      float magnitude is the target magnitude of the vectors
 */
float2* cudaBuffer(int length, float magnitude);
