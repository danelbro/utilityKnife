#pragma once

#include <random>

namespace utl {

struct Vec2d {
    Vec2d() : x{}, y{} {}
    Vec2d(double new_x, double new_y) : x{new_x}, y{new_y} {}
    Vec2d(float new_x, float new_y) : x{new_x}, y{new_y} {}
    Vec2d(int new_x, int new_y)
        : x{static_cast<double>(new_x)}, y{static_cast<double>(new_y)}
    {}
    Vec2d(double angle);

    Vec2d& operator=(double angle);

    double magnitude() const;
    double magnitude_squared() const;
    double angle() const;
    double angleDeg() const;
    double angleTo(const Vec2d& otherVec) const;

    Vec2d operator*(const double& x) const;
    // dot product
    double operator*(const Vec2d& otherVec) const;

    Vec2d operator/(const double& x) const;

    Vec2d operator+(const Vec2d& otherVec) const;

    Vec2d operator+=(const Vec2d& otherVec);

    Vec2d operator-() const;
    Vec2d operator-(const Vec2d& otherVec) const;

    Vec2d normalize() const;
    void normalizeInPlace();
    Vec2d update(double new_x, double new_y);
    Vec2d update(Vec2d new_vec);
    Vec2d rotate_rad(double rad);
    void rotate_rad_ip(double rad);
    Vec2d rotate_deg(double deg);
    void rotate_deg_ip(double deg);

    static double convert_x_elem_from_rad(double rad);
    static double convert_y_elem_from_rad(double rad);
    static Vec2d convert_from_degrees(double deg);
    static Vec2d convert_from_rad(double rad);

    double x{};
    double y{};
};

Vec2d randomPos(std::mt19937& rng, int w, int h);

}  // namespace utl
