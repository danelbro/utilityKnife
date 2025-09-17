#pragma once

#include <random>

namespace utl {

class RNG {
public:
    RNG();

    std::mt19937& rng();
    void seed();

private:
    std::mt19937 m_rng;
};

}  // namespace utl
