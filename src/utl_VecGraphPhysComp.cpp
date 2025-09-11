#include "utl_VecGraphPhysComp.hpp"

#include "utl_Stage.hpp"
#include "utl_VecGraphPhysEnt.hpp"
#include "utl_VectorDraw.hpp"

namespace utl {

VecGraphPhysComp::VecGraphPhysComp(double mass, VecGraphPhysEnt* new_owner)
    : m_owner{new_owner}, m_mass{mass}
{}

double VecGraphPhysComp::mass() const
{
    return m_mass;
}

double VecGraphPhysComp::impulse() const
{
    return m_impulse;
}

Vec2d VecGraphPhysComp::acceleration() const
{
    return m_acceleration;
}

Vec2d VecGraphPhysComp::velocity() const
{
    return m_velocity;
}

Vec2d VecGraphPhysComp::velocityAngleVector() const
{
    return m_velocityVector;
}

double VecGraphPhysComp::velocityAngle() const
{
    return m_velocityAngle;
}

double VecGraphPhysComp::facing() const
{
    return m_facingAngle;
}

Vec2d VecGraphPhysComp::facingVec() const
{
    return m_facingVector;
}

VecGraphPhysEnt* VecGraphPhysComp::owner()
{
    return m_owner;
}

void VecGraphPhysComp::turn(double turnSpeed, double dt)
{
    set_facing_angle(m_facingAngle + turnSpeed * dt);
}

void VecGraphPhysComp::setMass(double mass)
{
    m_mass = mass;
}

void VecGraphPhysComp::setFrameImpulse(double power)
{
    m_impulse = power;
}

void VecGraphPhysComp::setAcceleration(Vec2d accel)
{
    m_acceleration = accel;
}

void VecGraphPhysComp::setVelocity(Vec2d vel)
{
    m_velocity = vel;
}

void VecGraphPhysComp::set_facing_angle(double angle)
{
    if (angle < 0) {
        m_facingAngle = 360 + angle;
    } else if (angle >= 360) {
        m_facingAngle = angle - 360;
    } else {
        m_facingAngle = angle;
    }

    m_facingVector = m_facingAngle;
}

void VecGraphPhysComp::set_facing_angle(Vec2d newVector)
{
    m_facingVector = newVector;
    set_facing_angle(newVector.angleDeg());
}

void VecGraphPhysComp::setOwner(VecGraphPhysEnt* new_owner)
{
    m_owner = new_owner;
}

void VecGraphPhysComp::update(double dt)
{
    Vec2d totalForces{m_facingVector * m_impulse};
    m_acceleration = (totalForces / m_mass) * dt;
    m_velocity += m_acceleration * dt;

    if (m_owner->drawWrapped()) {
        m_owner->set_pos(
            wrap(m_owner->pos() + m_velocity * dt, m_owner->stage().screen()));
    } else {
        m_owner->set_pos(m_owner->pos() + m_velocity * dt);
    }

    m_velocityVector = m_velocity.normalize();
    m_velocityAngle = m_velocityVector.angleDeg();
    m_impulse = 0;
}

}  // namespace utl
