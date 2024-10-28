#pragma once

#include <random>

namespace utl {

class RNG {
public:
    RNG();

    std::mt19937& rng() { return m_rng; }

private:
    std::random_device randDev;
    std::mt19937 m_rng;
};

}  // namespace utl
