#include "PhysicsComponent.hpp"

#include <cmath>
#include <numbers>

#include "PhysicsEntity.hpp"
#include "VectorDraw.hpp"

namespace utl {
void VecGraphPhysComp::turn(double turnSpeed, double dt)
{
    m_angle += turnSpeed * dt;

    if (m_angle < 0) {
        m_angle = 360 + m_angle;
    } else if (m_angle >= 360) {
        m_angle -= 360;
    }
}

void VecGraphPhysComp::update(double dt)
{
    m_dirVector = Vec2d{ std::sin((m_angle * std::numbers::pi) / 180.0), -std::cos((m_angle * std::numbers::pi) / 180.0) };
    Vec2d totalForces{ m_dirVector * m_impulse };
    m_acceleration = (totalForces / m_mass) * dt;
    m_velocity += m_acceleration * dt;
    m_owner->pos() += m_velocity * dt;
    if ( m_owner->drawWrapped() ) {
        wrap(m_owner->pos(), m_owner->screen());
    }
    m_impulse = 0;
}

void VecGraphPhysComp::setAngle(double angle)
{
    if (angle < 0) {
        m_angle = 360 + angle;
    } else if (angle >= 360) {
        m_angle = angle - 360;
    } else {
        m_angle = angle;
    }

    m_dirVector = Vec2d{ angle };
}

void VecGraphPhysComp::setAngle(Vec2d angle)
{
    m_dirVector = angle;
    setAngle(angle.angleDeg());
}

} // namespace utl
