#pragma once

#include <fstream>
#include <random>

struct Vec2d;

namespace utl {
    extern std::ofstream errorLogger;

    Vec2d randomPos(std::mt19937& rng, int w, int h);

    std::mt19937 makeSeededRNG();
}
