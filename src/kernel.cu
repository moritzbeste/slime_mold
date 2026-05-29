#include <cstdio>

__global__ void hello_kernel()
{
    printf("Hello from CUDA thread: block=%d thread=%d\n",
           blockIdx.x, threadIdx.x);
}

void launch_hello_kernel()
{
    hello_kernel<<<2, 4>>>();
    cudaDeviceSynchronize();
}