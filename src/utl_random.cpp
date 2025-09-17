#include "utl_random.hpp"

// #include <algorithm>
// #include <array>
// #include <functional>
#include <chrono>
#include <random>

namespace utl {

RNG::RNG()
    : m_rng{static_cast<std::mt19937::result_type>(
          std::chrono::system_clock::now().time_since_epoch().count())}
{
    // https://www.reddit.com/r/cpp/comments/r2k6xl/comment/hm6j348/
    // std::random_device randDev{};
    // std::array<unsigned int, std::mt19937::state_size> seed{};
    // std::generate_n(seed.data(), seed.size(), std::ref(randDev));
    // std::seed_seq seq(seed.begin(), seed.end());
    // m_rng.seed(seq);
}

std::mt19937& RNG::rng()
{
    return m_rng;
}

}  // namespace utl
