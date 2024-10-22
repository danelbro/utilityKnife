#pragma once

#include <random>

#ifndef NDEBUG
#include <iostream>
#define LOG(x) std::cout << x
#define ERRLOG(x) std::cerr << x
#else
#define LOG(x)
#define ERRLOG(x)
#endif

namespace utl {

std::mt19937 makeSeededRNG();

}  // namespace utl
