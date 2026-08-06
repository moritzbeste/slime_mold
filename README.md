# slime_mold
to build:
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/gcc15.cmake
cd build
cmake --build .

to run
./slime_mold