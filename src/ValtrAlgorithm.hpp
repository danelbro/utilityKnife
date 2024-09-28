#include <random>
#include <vector>

#include "Vec2d.hpp"

namespace utl {
    std::vector<Vec2d> genRandConvexPolygon(int n, const double& radius,
                                            std::mt19937& rng);
} // namespace utl
