#include "utility.h"
#include "config.h"

#include <cstdlib>

void InitRandom() {
    srand(Config::seed);
}

float RandomFloat() {
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return r;
}