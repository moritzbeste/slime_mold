#include <iostream>
#include "kernel.cuh"

int main()
{
    std::cout << "Launching CUDA kernel...\n";

    launch_hello_kernel();

    std::cout << "Done.\n";
    return 0;
}