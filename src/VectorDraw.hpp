#pragma once

/**
 * Drawing and related functions for vector graphics - wrapping, collision, etc.
 */

#include <vector>

#include "SDL_Interface.hpp"

namespace utl {

struct Box;
struct Vec2d;

    void wrap(Vec2d& pos, const Box& bounds);

    void DrawWrapLine(Renderer& rend, const Box& screen, double x1, double y1, double x2, double y2);

    bool isPointInPolygon(const Vec2d& point, const std::vector<Vec2d>& polygon);

    void ScanFill(const Box& screen, const std::vector<Vec2d>& poly, const Colour& col, Renderer& renderer);

    bool areColliding_SAT(const std::vector<Vec2d>& shape1, const std::vector<Vec2d>& shape2);
}
