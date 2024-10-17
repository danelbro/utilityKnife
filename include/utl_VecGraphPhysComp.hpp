#pragma once

#include "utl_Vec2d.hpp"

namespace utl {

class VecGraphPhysEnt;

/**
 * A component to be stored on a vector graphics physics entity - the component
 * stores state and runs physics calculations when update() is called.
 */
class VecGraphPhysComp {
public:
    VecGraphPhysComp(double mass, VecGraphPhysEnt* new_owner)
        : m_mass{mass}, m_impulse{0.0}, m_acceleration{0.0, 0.0},
          m_velocity{0, 0}, m_angle{0.0}, m_dirVector{0, 0}, m_owner{new_owner}
    {}

    double mass() const { return m_mass; }
    double impulse() const { return m_impulse; }
    Vec2d acceleration() const { return m_acceleration; }
    Vec2d velocity() const { return m_velocity; }
    double facing() const { return m_angle; }
    Vec2d facingVec() const { return m_dirVector; }

    VecGraphPhysEnt* owner() const { return m_owner; }

    void turn(double turnSpeed, double dt);
    void setMass(double mass) { m_mass = mass; }
    void setFrameImpulse(double power) { m_impulse = power; }
    void setAcceleration(Vec2d accel) { m_acceleration = accel; }
    void setVelocity(Vec2d vel) { m_velocity = vel; }
    void setAngle(double angle);
    void setAngle(Vec2d angle);

    void setOwner(VecGraphPhysEnt* new_owner) { m_owner = new_owner; }

    void update(double dt);

private:
    double m_mass;
    double m_impulse;
    Vec2d m_acceleration;
    Vec2d m_velocity;

    double m_angle;     // between 0 and 360
    Vec2d m_dirVector;  // vector representation of m_angle

    VecGraphPhysEnt* m_owner;
};

}  // namespace utl
