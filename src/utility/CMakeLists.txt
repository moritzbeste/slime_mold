find_package(CUDAToolkit REQUIRED)

add_library(utility STATIC
    utility.cpp
)

target_include_directories(utility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_link_libraries(utility PUBLIC
    CUDA::cudart
    CUDA::cuda_driver
)