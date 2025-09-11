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
    VecGraphPhysComp() = default;
    VecGraphPhysComp(double mass, VecGraphPhysEnt* new_owner);

    double mass() const;
    double impulse() const;
    Vec2d acceleration() const;
    Vec2d velocity() const;
    Vec2d velocityAngleVector() const;
    double velocityAngle() const;
    double facing() const;
    Vec2d facingVec() const;

    VecGraphPhysEnt* owner();

    void turn(double turnSpeed, double dt);
    void setMass(double mass);
    void setFrameImpulse(double power);
    void setAcceleration(Vec2d accel);
    void setVelocity(Vec2d vel);
    void set_facing_angle(double angle);
    void set_facing_angle(Vec2d angle);

    void setOwner(VecGraphPhysEnt* new_owner);

    void update(double dt);

private:
    VecGraphPhysEnt* m_owner{nullptr};

    double m_mass{};
    double m_impulse{0.0};
    Vec2d m_acceleration{0.0, 0.0};
    Vec2d m_velocity{0.0, 0.0};

    double m_facingAngle{0.0};  // between 0 and 360
    Vec2d m_facingVector{0.0, 0.0};  // vector representation of m_angle

    Vec2d m_velocityVector{0.0, 0.0};
    double m_velocityAngle{0.0};
};

}  // namespace utl
