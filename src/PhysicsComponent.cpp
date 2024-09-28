#include "PhysicsComponent.hpp"

#include <cmath>

#include "Entity.hpp"
#include "GameWorld.hpp"
#include "VectorDraw.hpp"

void PhysicsComponent::turn(double turnSpeed, double dt)
{
    m_angle += turnSpeed * dt;

    if (m_angle < 0)
        m_angle = 360 + m_angle;
    else if (m_angle >= 360)
        m_angle -= 360;
}

void PhysicsComponent::update(double dt)
{
    m_dir_vector = { std::sin((m_angle * M_PI) / 180),
                     -std::cos((m_angle * M_PI) / 180) };
    auto totalForces{ m_dir_vector * m_impulse };
    m_acceleration = (totalForces / m_mass) * dt;
    m_velocity += m_acceleration * dt;
    m_owner->pos() += m_velocity * dt;
    utl::wrap(m_owner->pos(), m_owner->gameWorld.screen);
    m_impulse = 0;
}

void PhysicsComponent::setAngle(double angle)
{
    if (angle < 0)
        m_angle = 360 + angle;
    else if (angle >= 360)
        m_angle = angle - 360;
    else
        m_angle = angle;
}
