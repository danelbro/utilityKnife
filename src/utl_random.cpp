#include "utl_random.hpp"

#include <ctime>
#include <random>

namespace utl {

RNG::RNG() : randDev{}, m_rng{randDev()}
{
    m_rng.seed(static_cast<unsigned long>(std::time(nullptr)));
}

}  // namespace utl
