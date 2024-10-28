#include "utl_Vec2d.hpp"
#include "utl_random.hpp"

#include <cmath>
#include <numbers>

namespace utl {

Vec2d::Vec2d(double angleDeg)
    : x{convert_x_elem_from_rad(angleDeg / 180.0)},
      y{convert_y_elem_from_rad(angleDeg / 180.0)}
{}

Vec2d& Vec2d::operator=(double angleDeg)
{
    x = convert_x_elem_from_rad(angleDeg / 180.0);
    y = convert_y_elem_from_rad(angleDeg / 180.0);
    return *this;
}

double Vec2d::magnitude() const
{
    return std::sqrt(magnitude_squared());
}

double Vec2d::magnitude_squared() const
{
    return x * x + y * y;
}

double Vec2d::angle() const
{
    Vec2d normVec = this->normalize();
    return std::numbers::pi - std::atan2(normVec.x, normVec.y);
}

double Vec2d::angleDeg() const
{
    return angle() * 180 / std::numbers::pi;
}

double Vec2d::angleTo(const Vec2d& otherVec) const
{
    return std::acos((*this * otherVec)
                     / (this->magnitude() * otherVec.magnitude()));
}

void Vec2d::normalizeInPlace()
{
    auto mag = magnitude();
    if (mag != 0) {
        x /= mag;
        y /= mag;
    }
}

Vec2d Vec2d::normalize() const
{
    auto mag = magnitude();
    if (mag == 0) {
        return *this;
    }
    return Vec2d{x / mag, y / mag};
}

Vec2d Vec2d::update(double new_x, double new_y)
{
    x = new_x;
    y = new_y;

    return *this;
}

Vec2d Vec2d::update(Vec2d new_vec)
{
    return update(new_vec.x, new_vec.y);
}

Vec2d Vec2d::rotate_rad(double rad)
{
    double new_x = (x * std::cos(rad)) - (y * std::sin(rad));
    double new_y = (x * std::sin(rad)) + (y * std::cos(rad));

    return Vec2d{new_x, new_y};
}

void Vec2d::rotate_rad_ip(double rad)
{
    x = (x * std::cos(rad)) - (y * std::sin(rad));
    y = (x * std::sin(rad)) + (y * std::cos(rad));
}

Vec2d Vec2d::rotate_deg(double deg)
{
    return rotate_rad(deg * (std::numbers::pi / 180.0));
}

void Vec2d::rotate_deg_ip(double deg)
{
    rotate_rad_ip(deg * (std::numbers::pi / 180.0));
}

Vec2d Vec2d::operator*(const double& n) const
{
    return Vec2d{x * n, y * n};
}

double Vec2d::operator*(const Vec2d& otherVec) const
{
    return x * otherVec.x + y * otherVec.y;
}

Vec2d Vec2d::operator/(const double& n) const
{
    return Vec2d{x / n, y / n};
}

Vec2d Vec2d::operator+(const Vec2d& otherVec) const
{
    return Vec2d{x + otherVec.x, y + otherVec.y};
}

Vec2d Vec2d::operator+=(const Vec2d& otherVec)
{
    *this = *this + otherVec;
    return *this;
}

Vec2d Vec2d::operator-() const
{
    return Vec2d{*this * -1};
}

Vec2d Vec2d::operator-(const Vec2d& otherVec) const
{
    return Vec2d{x - otherVec.x, y - otherVec.y};
}

double Vec2d::convert_x_elem_from_rad(double rad)
{
    return std::sin(rad * std::numbers::pi);
}

double Vec2d::convert_y_elem_from_rad(double rad)
{
    return -std::cos(rad * std::numbers::pi);
}

Vec2d Vec2d::convert_from_degrees(double deg)
{
    return {convert_x_elem_from_rad(deg / 180.0),
            convert_y_elem_from_rad(deg / 180.0)};
}

Vec2d Vec2d::convert_from_rad(double rad)
{
    return {convert_x_elem_from_rad(rad), convert_y_elem_from_rad(rad)};
}

Vec2d randomPos(utl::RNG& rng, int w, int h)
{
    std::uniform_real_distribution<double> xDist(0.0, static_cast<double>(w));
    std::uniform_real_distribution<double> yDist(0.0, static_cast<double>(h));

    return {xDist(rng), yDist(rng)};
}

}  // namespace utl
