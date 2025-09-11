#include "utl_random.hpp"

#include <ctime>
#include <random>

namespace utl {

RNG::RNG()
{
    m_rng.seed(static_cast<unsigned long>(std::time(nullptr)));
}

std::mt19937& RNG::rng()
{
    return m_rng;
}

}  // namespace utl
