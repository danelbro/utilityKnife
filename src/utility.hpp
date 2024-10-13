#pragma once

#include <fstream>
#include <random>

namespace utl {

extern std::ofstream errorLogger;

std::mt19937 makeSeededRNG();

}  // namespace utl
