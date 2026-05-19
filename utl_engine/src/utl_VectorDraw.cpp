#include "utl_VectorDraw.hpp"

#include "utl_Box.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

namespace utl {

static double wrapCoord(double coordinate, double upperBound)
{
    if (coordinate < 0) {
        return coordinate + upperBound;
    } else if (coordinate > upperBound) {
        return coordinate - upperBound;
    } else {
        return coordinate;
    }
}

Vec2d wrap(const Vec2d& pos, const Box& screen)
{
    return Vec2d{wrapCoord(pos.x, screen.w), wrapCoord(pos.y, screen.h)};
}

void DrawWrapLine(utl::Renderer& rend, const Box& screen, const Vec2d& point1,
                  const Vec2d& point2)
{
    double x{};
    double y{};
    Vec2d p1{point1};
    Vec2d p2{point2};
    double dy{p2.y - p1.y};
    double dx{p2.x - p1.y};
    double tiny{1.0};  // eventually the lines are drawn onto discrete pixels

    int xwrap{screen.w};
    int ywrap{screen.h};

    std::vector<Vec2d> points{};
    points.reserve(0xFFF);

    if (std::abs(dx) < tiny) {
        if (p1.y > p2.y) {
            y = p2.y;
            p2.y = p1.y;
            p1.y = y;
        }
        for (y = p1.y; y <= p2.y; ++y) {
            points.emplace_back(wrapCoord(p1.x, xwrap), wrapCoord(y, ywrap));
        }
    } else {
        double m{dy / dx};
        double c{p1.y - (m * p1.x)};
        if (-1 <= m && m <= 1) {
            if (p1.x > p2.x) {
                x = p2.x;
                p2.x = p1.x;
                p1.x = x;
            }
            for (x = p1.x; x <= p2.x; ++x) {
                y = (m * x) + c;
                points.emplace_back(wrapCoord(x, xwrap), wrapCoord(y, ywrap));
            }
        } else {
            if (p1.y > p2.y) {
                y = p2.y;
                p2.y = p1.y;
                p1.y = y;
            }
            for (y = p1.y; y <= p2.y; ++y) {
                x = (y - c) / m;
                points.emplace_back(wrapCoord(x, xwrap), wrapCoord(y, ywrap));
            }
        }
    }

    for (const auto& point : points) {
        drawPoint(rend, point.x, point.y);
    }
}

// adatpted from https://alienryderflex.com/polygon/
bool isPointInPolygon(const Vec2d& point, const std::vector<Vec2d>& polygon)
{
    size_t i{0}, j{polygon.size() - 1};
    bool oddNodes{false};

    for (i = 0; i < polygon.size(); i++) {
        if ((polygon[i].y < point.y && polygon[j].y >= point.y)
            || (polygon[j].y < point.y && polygon[i].y >= point.y)) {
            if (polygon[i].x
                    + (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y)
                          * (polygon[j].x - polygon[i].x)
                < point.x) {
                oddNodes = !oddNodes;
            }
        }
        j = i;
    }
    return oddNodes;
}

// adapted frpm https://alienryderflex.com/polygon_fill/
void ScanFill(const Box& screen, const std::vector<Vec2d>& poly,
              const Colour& col, Renderer& renderer)
{
    Colour old{getRendererDrawColour(renderer)};
    setRendererDrawColour(renderer, col);
    size_t polySize = poly.size();

    std::vector<double> ys{};
    ys.reserve(polySize);
    for (const auto& p : poly) {
        ys.emplace_back(p.y);
    }

    const double& y_min{*std::ranges::min_element(ys)};
    const double& y_max{*std::ranges::max_element(ys)};

    Vec2d pixel{};
    for (pixel.y = y_min; pixel.y < y_max; pixel.y++) {
        size_t i{};
        size_t j{polySize - 1};
        std::vector<double> nodesX;
        nodesX.reserve(polySize);

        for (i = 0; i < polySize; i++) {
            if ((poly[i].y < pixel.y && poly[j].y >= pixel.y)
                || (poly[j].y < pixel.y && poly[i].y >= pixel.y)) {
                nodesX.emplace_back(poly[i].x
                                    + (pixel.y - poly[i].y)
                                          / (poly[j].y - poly[i].y)
                                          * (poly[j].x - poly[i].x));
            }
            j = i;
        }

        std::ranges::sort(nodesX);

        size_t nodesXSize{nodesX.size()};
        for (i = 0; i < nodesXSize; i += 2) {
            for (pixel.x = nodesX[i]; pixel.x < nodesX.at(i + 1);
                 pixel.x += 1) {
                DrawWrapLine(renderer, screen, pixel,
                             {nodesX.at(i + 1), pixel.y});
            }
        }
    }
    setRendererDrawColour(renderer, old);
}

static void populateNormals(const std::vector<Vec2d>& shape,
                            std::vector<Vec2d>& axes)
{
    size_t i{};
    auto size{shape.size()};
    for (i = 0; i < size; ++i) {
        axes.emplace_back(Vec2d{-(shape[(i + 1) % size].y - shape[i].y),
                                shape[(i + 1) % size].x - shape[i].x}
                              .normalize());
    }
}

bool areColliding_SAT(const std::vector<Vec2d>& shape1,
                      const std::vector<Vec2d>& shape2)
{
    auto shape1size = shape1.size();
    auto shape2size = shape2.size();

    std::vector<Vec2d> axes{};
    axes.reserve(shape1size + shape2size);

    populateNormals(shape1, axes);
    populateNormals(shape2, axes);

    // project shapes onto axes
    for (const auto& axis : axes) {
        double shape1min{INFINITY}, shape1max{-INFINITY};
        double shape2min{INFINITY}, shape2max{-INFINITY};

        for (const auto& p : shape1) {
            double q = p * axis;
            shape1min = std::min(q, shape1min);
            shape1max = std::max(q, shape1max);
        }

        for (const auto& p : shape2) {
            double q = p * axis;
            shape2min = std::min(q, shape2min);
            shape2max = std::max(q, shape2max);
        }

        if (!(shape2max > shape1min && shape1max > shape2min)) {
            return false;
        }
    }
    return true;
}

}  // namespace utl
