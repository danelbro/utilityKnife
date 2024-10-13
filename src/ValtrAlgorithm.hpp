#include "Vec2d.hpp"

#include <random>
#include <vector>

namespace utl {

std::vector<Vec2d> genRandConvexPolygon(int n, const double& radius,
                                        std::mt19937& rng);

}  // namespace utl
