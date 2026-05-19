#pragma once

#include <random>

namespace utl {

struct RNG {
private:
    std::random_device rand_dev;

public:
    RNG();
    std::mt19937 rng;
};

}  // namespace utl
