#include "utility.hpp"

#include <chrono>

#include "Vec2d.hpp"

namespace utl {
    std::ofstream errorLogger("exception.log");

    Vec2d randomPos(std::mt19937& rng, int w, int h)
    {
        std::uniform_real_distribution<double> xDist(0.0,
                                                     static_cast<double>(w));
        std::uniform_real_distribution<double> yDist(0.0,
                                                     static_cast<double>(h));

        return { xDist(rng), yDist(rng) };
    }

    std::mt19937 makeSeededRNG()
    {
        std::random_device randDev;
        auto rng = std::mt19937{ randDev() };
        std::mt19937::result_type seed_val{
            static_cast<unsigned long>(std::time(nullptr))
        };
        rng.seed(seed_val);

        return rng;
    }
}
