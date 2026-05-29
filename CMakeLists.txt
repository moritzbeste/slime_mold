cmake_minimum_required(VERSION 3.18)

project(slime_mold LANGUAGES CXX CUDA)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(slime_mold)

add_subdirectory(src)

target_link_directories(slime_mold PRIVATE
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/lib
)

target_include_directories(slime_mold PRIVATE
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_link_libraries(slime_mold PRIVATE
    raylib
    OpenGL::GL
)

set_target_properties(slime_mold PROPERTIES
    BUILD_RPATH
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/lib
)