/*
 * MIT License
 *
 * Copyright (c) 2017 Sander Verdonschot
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Note from Dan Elbro (2023): I adapted this algorithm from a Java
// algorithm found at the following page:
// https://cglab.ca/~sander/misc/ConvexGeneration/convex.html

#include "ValtrAlgorithm.hpp"

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

#include "Vec2d.hpp"

namespace utl {
    std::vector<Vec2d> genRandConvexPolygon(int m, const double& radius,
                                            std::mt19937& rng)
    {
        size_t n = static_cast<size_t>(m);
        std::uniform_real_distribution<double> dist(-radius, radius);
        std::uniform_int_distribution<int> coinFlip(0, 1);
        std::vector<double> xPool{}, yPool{};
        xPool.reserve(n); yPool.reserve(n);

        for (size_t i{ 0 }; i < n; ++i) {
            xPool.emplace_back(dist(rng));
            yPool.emplace_back(dist(rng));
        }

        std::sort(xPool.begin(), xPool.end());
        std::sort(yPool.begin(), yPool.end());

        double minX = xPool.front(), maxX = xPool.back();
        double minY = yPool.front(), maxY = yPool.back();

        std::vector<double> xVec{}, yVec{};
        xVec.reserve(n); yVec.reserve(n);

        double lastTop{ minX }, lastBot{ minX };

        for (size_t i{ 1 }; i < n-1; ++i) {
            double x = xPool[i];

            if (coinFlip(rng)) {
                xVec.emplace_back(x - lastTop);
                lastTop = x;
            } else {
                xVec.emplace_back(lastBot - x);
                lastBot = x;
            }
        }

        xVec.emplace_back(maxX - lastTop);
        xVec.emplace_back(lastBot - maxX);

        double lastLeft{ minY }, lastRight{ minY };

        for (size_t i{ 1 }; i < n-1; ++i) {
            double y = yPool[i];

            if (coinFlip(rng)) {
                yVec.emplace_back(y - lastLeft);
                lastLeft = y;
            } else {
                yVec.emplace_back(lastRight - y);
                lastRight = y;
            }
        }

        yVec.emplace_back(maxY - lastLeft);
        yVec.emplace_back(lastRight - maxY);

        std::shuffle(yVec.begin(), yVec.end(), rng);
        std::vector<Vec2d> vec{};
        vec.reserve(n);

        for (size_t i{ 0 }; i < n; ++i)
            vec.emplace_back(Vec2d{xVec[i], yVec[i]});

        std::sort(vec.begin(), vec.end(),
                  [] (const Vec2d& lhs, const Vec2d& rhs) {
                      return lhs.angle() < rhs.angle();
                  }
            );

        double x{ 0.0 }, y{ 0.0 };
        double minPolygonX{ 0.0 }, minPolygonY{ 0.0 };
        std::vector<Vec2d> points{};
        points.reserve(n);

        for (size_t i{ 0 }; i < n; ++i) {
            points.emplace_back(Vec2d{ x, y });

            x += vec[i].x;
            y += vec[i].y;

            minPolygonX = std::min(minPolygonX, x);
            minPolygonY = std::min(minPolygonY, y);
        }

        double xShift{ minX - minPolygonX }, yShift{ minY - minPolygonY };

        for (size_t i{ 0 }; i < n; ++i) {
            Vec2d p{ points[i] };
            points[i] = Vec2d{p.x + xShift, p.y + yShift};
        }

        return points;
    }
} // namespace utl
