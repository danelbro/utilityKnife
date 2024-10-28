#pragma once

#include "utl_Vec2d.hpp"

#include "utl_random.hpp"
#include <vector>

namespace utl {

std::vector<Vec2d> genRandConvexPolygon(int n, const double& radius,
                                        utl::RNG& rng);

}  // namespace utl
