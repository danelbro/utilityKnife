#include "utl_random.hpp"

#include <ctime>
#include <random>

namespace utl {

RNG::RNG()

    : rand_dev{}, rng{rand_dev()}
{}

}  // namespace utl
