#include "utl_VecGraphPhysComp.hpp"

#include "utl_VecGraphPhysEnt.hpp"
#include "utl_VectorDraw.hpp"

namespace utl {
void VecGraphPhysComp::turn(double turnSpeed, double dt)
{
    set_facing_angle(m_facingAngle + turnSpeed * dt);
}

void VecGraphPhysComp::update(double dt)
{
    Vec2d totalForces{m_facingVector * m_impulse};
    m_acceleration = (totalForces / m_mass) * dt;
    m_velocity += m_acceleration * dt;

    if (m_owner->drawWrapped()) {
        m_owner->set_pos(
            wrap(m_owner->pos() + m_velocity * dt, m_owner->screen()));
    } else {
        m_owner->set_pos(m_owner->pos() + m_velocity * dt);
    }

    m_velocityVector = m_velocity.normalize();
    m_velocityAngle = m_velocityVector.angleDeg();
    m_impulse = 0;
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

}  // namespace utl
