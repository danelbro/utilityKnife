#pragma once

/**
 * Drawing and related functions for vector graphics - wrapping, collision, etc.
 */

#include "utl_Box.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <vector>

namespace utl {

Vec2d wrap(const Vec2d& pos, const Box& bounds);

void DrawWrapLine(Renderer& rend, const Box& screen, const Vec2d& point1,
                  const Vec2d& point2);

bool isPointInPolygon(const Vec2d& point, const std::vector<Vec2d>& polygon);

void ScanFill(const Box& screen, const std::vector<Vec2d>& poly,
              const Colour& col, Renderer& renderer);

bool areColliding_SAT(const std::vector<Vec2d>& shape1,
                      const std::vector<Vec2d>& shape2);
}  // namespace utl
