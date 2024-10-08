#pragma once

#include "Box.hpp"

namespace utl {

/**
 * A struct to represent an area an entity moves in - bounded by the screen,
 * but with a fluid density for physics calculations
 */
struct GameWorld {
    Box screen{ };
    double fluidDensity{ };
};

} // namespace utl
