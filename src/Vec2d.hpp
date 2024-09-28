#pragma once

struct Vec2d {
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

    double x{ };
    double y{ };
};
