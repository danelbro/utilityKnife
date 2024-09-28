#pragma once

#include <vector>

#include "SDL_Interface.hpp"

struct Box;
struct GameWorld;
struct Vec2d;

namespace utl {
    void wrap(Vec2d& pos, const Box& bounds);

    void DrawWrapLine(Renderer& rend, const Box& screen,
                      double x1, double y1,
                      double x2, double y2);

    bool PointInPolygon(const Vec2d& point, const std::vector<Vec2d>& polygon);

    void ScanFill(const GameWorld& gw, const std::vector<Vec2d>& poly,
                  const Colour& col, Renderer& renderer);

    bool areColliding_SAT(const std::vector<Vec2d>& shape1,
                          const std::vector<Vec2d>& shape2);
}
