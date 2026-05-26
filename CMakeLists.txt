cmake_minimum_required(VERSION 3.15)

project(slime_mold)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(slime_mold)

add_subdirectory(src)

target_link_directories(slime_mold PRIVATE
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/lib
)

target_include_directories(slime_mold PRIVATE
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/include
)

target_link_libraries(slime_mold PRIVATE
    raylib
    OpenMP::OpenMP_CXX
)

set_target_properties(slime_mold PROPERTIES
    BUILD_RPATH
    ${CMAKE_SOURCE_DIR}/external/raylib-6.0_linux_amd64/lib
)