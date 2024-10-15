#include "utl_utility.hpp"

#include <ctime>

namespace utl {

std::ofstream errorLogger("exception.log");

std::mt19937 makeSeededRNG()
{
    std::random_device randDev;
    auto rng = std::mt19937{randDev()};
    std::mt19937::result_type seed_val{
        static_cast<unsigned long>(std::time(nullptr))};
    rng.seed(seed_val);

    return rng;
}

}  // namespace utl
